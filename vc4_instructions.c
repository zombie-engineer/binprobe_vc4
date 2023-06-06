#include "vc4_instructions.h"
#include "vc4_scalar_op.h"
#include "vc4_float_op.h"
#include "vc4_disas.h"
#include "symbol_table.h"
#include <stdio.h>

#define VC4_WORD 0
#define VC4_HALFWORD 1
#define VC4_BYTE 2
#define VC4_SIGNED_HALFWORD 3

static int vc4_operand_size(int format) {
	switch (format) {
		case VC4_WORD:
			return 4;
		case VC4_HALFWORD:
		case VC4_SIGNED_HALFWORD:
			return 2;
		case VC4_BYTE:
			return 1;
		default:
			return 0;
	}
}

struct vc4_cond {
	const char *name;
};

static struct vc4_cond vc4_cond_table[] = {
#define DECL(__a) { .name = #__a }
	DECL(eq),
	DECL(ne),
	DECL(cs),
	DECL(cc),
	DECL(mi),
	DECL(pl),
	DECL(vs),
	DECL(vc),
	DECL(hi),
	DECL(ls),
	DECL(ge),
	DECL(lt),
	DECL(gt),
	DECL(le),
	DECL(always),
	DECL(never),
#undef DECL
};

static const char *cond_to_str(int cond_idx)
{
	return vc4_cond_table[cond_idx].name;
}

static void fill_condstr(char *condstr, int cond)
{
	sprintf(condstr, ".%s", cond_to_str(cond));
	if (strcmp("always", cond_to_str(cond)) == 0) {
		condstr[0] = 0;
		condstr[1] = 0;
	}
}

static inline uint32_t sign_extend(uint32_t value, int bits)
{
	value &= (1 << bits) - 1;

	if (value & (1 << (bits - 1)))
		return value | ~((1 << bits) - 1);

	return value;
}

static void i80_arg_to_str(char *str, int n, int arg, int ra_x)
{
	int y;
	const char *char2 = "";
	const char *char1 = "H";
	int offs[] = { 0, 16, 32, 48, 0, 32, 0 };
	int off_idx;
	int x;
	int prefix;

#if 0
	snprintf(str, n, "%04x ", arg);
	str += 5;
	n -= 5;
#endif

	prefix = bf_extract(arg, 6, 4);
	if (prefix >= 0b1110) {
		snprintf(str, n, "-");
		return;
	}

	if (ra_x != -1 || bf_test(prefix, 0))
			y = bf_extract(arg, 0, 6);
	else
		y = bf_extract(arg, 4, 2) << 4;

	x = offs[bf_extract(prefix, 1, 3)];
	if (ra_x == -1)
		x += bf_extract(arg, 0, 4);
	else
		x += ra_x;

	if (bf_test(prefix, 3))
		char2 = bf_test(prefix, 2) ? "Y" : "X";

	if (bf_test(prefix, 0))
		char1 = "V";

	snprintf(str, n, "%s%s(%d,%d)",
		char1, char2, y, x);
}

static inline uint32_t get_pc(const struct vc4_disas *d, const uint16_t *instr_addr)
{
  return d->current_pc;
}

void print_abs_addr(struct vc4_disas *d, uint32_t addr, int as_primary)
{
	char addbuf[256];
	const struct symbol *s;
	addbuf[0] = 0;

	s = symbol_table_find_symbol(&d->symbols, addr);

	if (s)
		snprintf(addbuf, sizeof(addbuf), " <%s>", s->name);
	if (!as_primary)
		printf("(0x%08x)%s", addr, addbuf);
	else
		printf(", 0x%08x%s", addr, addbuf);
}

void vc4_i_print_i16_illegal(struct vc4_disas *d, uint16_t *instr)
{
	printf("illegal16");
}

void vc4_i_print_i32_p_control(struct vc4_disas *dis, uint16_t *instr)
{
	int a        = bf_extract(instr[1],  0, 5);
	int d        = bf_extract(instr[0],  0, 5);
	int p_assign = bf_extract(instr[0],  5, 1);
	printf("mov %s%d, %s%d",
		p_assign ? "p" : "r", d,
		p_assign ? "p" : "r", a);
}

void vc4_i_print_i32_stub(struct vc4_disas *d, uint16_t *instr)
{
	printf("i32_stub");
}

void vc4_i_print_i48_b(struct vc4_disas *d, uint16_t *instr)
{
	int    o = instr[2] << 16 | instr[1];
	int    l = bf_extract(instr[0], 9,  1);
	int is_b = bf_extract(instr[0], 8,  1);
	uint32_t new_pc;

	new_pc = get_pc(d, instr) + o;

	printf("%c%s %s0x%08x",
		is_b ? 'b' : 'j',
		l ? "l" : "",
		is_b ? "$+" : "",
		o);

	print_abs_addr(d, new_pc, 0);
}

void vc4_i_print_i48_add_pc_rel(struct vc4_disas *d, uint16_t *instr)
{
	int    o  = instr[2] << 16 | instr[1];
	int    rd = bf_extract(instr[0],  0,  5);
	printf("add r%d, pc, 0x%08x", rd, o);
	print_abs_addr(d, get_pc(d, instr) + o, 0);
}

void vc4_i_print_i48_add_ld_st(struct vc4_disas *d, uint16_t *instr)
{
	char operandstr[16];

	int    o  = instr[2] << 16 |
	            bf_extract(instr[1],  0, 11);
	int    rs = bf_extract(instr[1], 11,  5);
	int    rd = bf_extract(instr[0],  0,  5);
	int is_st = bf_extract(instr[0],  5,  1);
	int     w = bf_extract(instr[0],  6,  2);
	int is_pc = bf_extract(instr[0],  8,  1);

	if (is_pc)
		snprintf(operandstr, sizeof(operandstr), "pc");
	else
		snprintf(operandstr, sizeof(operandstr), "r%d", rs);

	printf("%s%d r%d, [%s + 0x%08x]",
		is_st ? "st" : "ld",
		vc4_operand_size(w), rd,
		operandstr,
		o);

	if (is_pc) {
		print_abs_addr(d, get_pc(d, instr) + o, 1);
	}
}

void vc4_i_print_i48_alu_op(struct vc4_disas *d, uint16_t *instr)
{
	uint32_t u = (instr[2] << 16) | instr[1];
	int rd = bf_extract(instr[0], 0, 5);
	int op = bf_extract(instr[0], 5, 5);
	printf("%s r%d", vc4_scalar_op_get_name(op), rd);
	print_abs_addr(d, u, 1);
}

void vc4_i_print_i48_alu_add(struct vc4_disas *d, uint16_t *instr)
{
	uint32_t u = (instr[2] << 16) | instr[1];
	int rd = bf_extract(instr[0], 0, 5);
	int rs = bf_extract(instr[0], 5, 5);
	printf("add r%d, r%d, 0x%08x", rd, rs, u);
	print_abs_addr(d, u, 1);
	if (rs == 24) {
		print_abs_addr(d, u + d->data_start, 0);
	}
}

void vc4_i_print_i48_stub(struct vc4_disas *d, uint16_t *instr)
{
	printf("i48_stub");
}


void vc4_i_print_i16_stub(struct vc4_disas *d, uint16_t *instr)
{
	printf("i16_stub");
}

void vc4_i_print_i16_bkpt(struct vc4_disas *d, uint16_t *instr) {
	printf("bkpt");
}

void vc4_i_print_i16_nop(struct vc4_disas *d, uint16_t *instr) {
	printf("nop");
}

void vc4_i_print_i16_sleep(struct vc4_disas *d, uint16_t *instr) {
	printf("sleep");
}

void vc4_i_print_i16_user(struct vc4_disas *d, uint16_t *instr) {
	printf("user");
}

void vc4_i_print_i16_ei(struct vc4_disas *d, uint16_t *instr) {
	// printf("SR |= 1<<30");
	printf("ei (enable interrupts)");
}

void vc4_i_print_i16_di(struct vc4_disas *d, uint16_t *instr) {
	printf("di (disable interrupts)");
}

void vc4_i_print_i16_cbclr(struct vc4_disas *d, uint16_t *instr) {
	printf("cbclr (sr & ~0b00110000)");
}

void vc4_i_print_i16_cbadd1(struct vc4_disas *d, uint16_t *instr) {
	printf("cbadd1");
}

void vc4_i_print_i16_cbadd2(struct vc4_disas *d, uint16_t *instr) {
	printf("cbadd2");
}

void vc4_i_print_i16_cbadd3(struct vc4_disas *d, uint16_t *instr) {
	printf("cbadd3");
}

void vc4_i_print_i16_rti(struct vc4_disas *d, uint16_t *instr) {
	printf("rti (return from interrupt) (pop pc)");
}

void vc4_i_print_i16_swi(struct vc4_disas *d, uint16_t *instr) {
	int Rd = bf_extract(instr[0], 0, 5);
	printf("swi r%d (0x20 + (r%d & 0x31) = %08x)", Rd, Rd, 0x20 + Rd);
}

void vc4_i_print_i16_b(struct vc4_disas *d, uint16_t *instr)
{
	int rd = bf_extract(instr[0], 0, 5);
	int l  = bf_extract(instr[0], 5, 1);
	printf("b%s r%d", l ? "l" : "", rd);
}

void vc4_i_print_i16_switch(struct vc4_disas *d, uint16_t *instr)
{
	int rd   = bf_extract(instr[0], 0, 4);
	int no_b = bf_extract(instr[0], 5, 1);
	printf("switch%s r%d", no_b ? "" : ".b", rd);
}

void vc4_i_print_i16_version(struct vc4_disas *d, uint16_t *instr)
{
	int rd = bf_extract(instr[0], 0, 5);
	printf("version r%d", rd);
}

void vc4_i_print_i16_swi_u(struct vc4_disas *d, uint16_t *instr)
{
	int u = bf_extract(instr[0], 0, 6);
	printf("swi 0x%08x", 0x20 + u);
}

/* stm / ldm */
void vc4_i_print_i16_ldm_stm(struct vc4_disas *d, uint16_t *instr)
{
	const int rb_select[]  = { 0, 6, 16, 24 };

	int m        = bf_extract(instr[0], 0, 5);
	int rb_index = bf_extract(instr[0], 5, 2);
	int is_store = bf_extract(instr[0], 7, 1);
	int lr_pc    = bf_extract(instr[0], 8, 1);

	int rb = rb_select[rb_index];
	int rm = (rb + m) & 0x1f;

	printf("%s r%d-r%d%s, [%s]",
		is_store ? "stm" : "ldm",
		rb,
		rm,
		lr_pc ? (is_store ? ", lr" : ", pc") : "",
		is_store ? "--sp" : "sp++");
}


void vc4_i_print_i16_ld_st_sp(struct vc4_disas *d, uint16_t *instr) {
	int rd    = bf_extract(instr[0], 0, 4);
	int off   = bf_extract(instr[0], 4, 5);
	int is_st = bf_extract(instr[0], 9, 1);
	off = off * 4;

	printf("%s r%d, [sp + %d]", is_st ? "st" : "ld", rd, off);
}

void vc4_i_print_i16_ldw_stw(struct vc4_disas *d, uint16_t *instr)
{
	int rd    = bf_extract(instr[0], 0, 4);
	int rs    = bf_extract(instr[0], 4, 4);
	int is_st = bf_extract(instr[0], 8, 1);
	int w     = bf_extract(instr[0], 9, 2);

	printf("%s%d r%d, [r%d]",
		is_st ? "st" : "ld",
		vc4_operand_size(w),
		rd,
		rs);
}

void vc4_i_print_i16_add_sp(struct vc4_disas *d, uint16_t *instr)
{
	int i  = bf_extract(instr[0], 5, 6);
	int rd = bf_extract(instr[0], 0, 5);
	i = sign_extend(i, 6) * 4;
	if (rd == 25)
		printf("add sp, %d", i);
	else
		printf("mov r%d, sp + 0x%08x", rd, i);
}

void vc4_i_print_i16_b_cond(struct vc4_disas *d, uint16_t *instr)
{
	char condstr[32];

	int32_t off   = bf_extract(instr[0], 0, 7);
	uint32_t cond = bf_extract(instr[0], 7, 4);

	off = sign_extend(off, 7) * 2;
	fill_condstr(condstr, cond);

	printf("b%s $+0x%04x", condstr, off);
	print_abs_addr(d, get_pc(d, instr) + off, 0);
}

void vc4_i_print_i16_ld_st(struct vc4_disas *d, uint16_t *instr)
{
	int rd    = bf_extract(instr[0], 0, 4);
	int rs    = bf_extract(instr[0], 4, 4);
	int u     = bf_extract(instr[0], 8, 4);
	int is_st = bf_extract(instr[0], 12, 1);

	u *= 4;
	printf("%s r%d, [r%d + 0x%08x]", is_st ? "st4" : "ld4", rd, rs, u);
}

void vc4_i_print_i16_alu_op(struct vc4_disas *d, uint16_t *instr) {
	/*
	 * 010o oooo ssss dddd
	 * 011o ooou uuuu dddd
	*/
	int imm_mode = bf_extract(instr[0], 13, 1);
	uint32_t Rd  = bf_extract(instr[0], 0, 4);
	uint32_t Rs;
	uint32_t op;
	uint32_t u;
	const char *opname;

	if (imm_mode) {
		u  = bf_extract(instr[0], 4, 5);
		op = bf_extract(instr[0], 9, 4);
		op *= 2;
	} else {
		Rs = bf_extract(instr[0], 4, 4);
		op = bf_extract(instr[0], 8, 5);
	}

	opname = vc4_scalar_op_get_name(op);

	if (imm_mode) {
		printf("%s r%d", opname, Rd);
		print_abs_addr(d, u, 1);
	} else
		printf("%s r%d, r%d", opname, Rd, Rs);
}

void vc4_i_print_i32_addcmpb(struct vc4_disas *d, uint16_t *instr)
{
	char condstr[32];
	int o;
	int u;
	int rs;
	int rd;
	int ra;
	int i;
	int cond;
	int mode;

	mode = bf_extract(instr[1], 14, 2);
	rd   = bf_extract(instr[0],  0, 4);
	cond = bf_extract(instr[0],  8, 4);
	fill_condstr(condstr, cond);

	if (mode == 0b00 || mode == 0b01) {
		o = bf_extract(instr[1],  0, 10);
		rs  = bf_extract(instr[1], 10,  4);
		ra  = bf_extract(instr[0],  4,  4);
		i   = sign_extend(ra, 4);
		o = sign_extend(o, 10) * 2;
		printf("addcmpb%s r%d, %s%d, r%d, $+0x%08x", condstr, rd,
			mode == 0b00 ? "r" : "",
			mode == 0b00 ? ra : i,
			rs, o);
	} else {
		/* modes 0b10, 0b11 */
		o = bf_extract(instr[1], 0, 8);
		u   = bf_extract(instr[1], 8, 6);
		ra  = bf_extract(instr[0], 4, 4);
		i   = sign_extend(ra, 4);
		o = sign_extend(o, 8) * 2;

		printf("addcmpb%s r%d, %s%d, %d, $+0x%08x", condstr, rd,
			mode == 0b10 ? "r" : "",
			mode == 0b10 ? ra : i,
			u, o);
	}

	print_abs_addr(d, (int)get_pc(d, instr) + o, 0);
}

void vc4_i_print_i32_b(struct vc4_disas *d, uint16_t *instr)
{
	char condstr[32];
	int cond;
	uint32_t o;
	int is_bl;
	uint32_t new_pc;

	o     = instr[1];
	is_bl = bf_extract(instr[0], 7, 1);
	o |= (bf_extract(instr[0], 0, 7) << 16);
	new_pc = get_pc(d, instr) + o;
	if (is_bl) {
		o |= bf_extract(instr[0], 8, 4) << 23;
		o = sign_extend(o, 27);
		// set_reg(lr, get_pc(d, instr) + 4);
	} else {
		o = sign_extend(o, 23);
		cond = bf_extract(instr[0], 8, 4);
		fill_condstr(condstr, cond);
	}
	o *= 2;
	new_pc = get_pc(d, instr) + o;

	printf("b%s $+0x%08x", is_bl ? "l" : condstr, o);
	print_abs_addr(d, new_pc, 0);
}

void vc4_i_print_i32_ld_st_cond_indexed(struct vc4_disas *d, uint16_t *instr)
{
	char condstr[32];
	char mulstr[32];

	int rb    = bf_extract(instr[1],  0, 5);
	int cond  = bf_extract(instr[1],  7, 4);
	int ra    = bf_extract(instr[1], 11, 5);
	int rd    = bf_extract(instr[0],  0, 5);
	int is_st = bf_extract(instr[0],  5, 1);
	int w     = bf_extract(instr[0],  6, 2);
	switch(w) {
		case 0b00: snprintf(mulstr, sizeof(mulstr), " * 4"); break;
		case 0b01: snprintf(mulstr, sizeof(mulstr), " * 2"); break;
		case 0b10: mulstr[0] = 0                           ; break;
		default:   snprintf(mulstr, sizeof(mulstr), " * 2X"); break;
	}

	fill_condstr(condstr, cond);
	printf("%s%d%s r%d, [r%d + r%d%s]",
		is_st ? "st" : "ld",
		vc4_operand_size(w),
		condstr,
		rd, ra, rb, mulstr);
}

void vc4_i_print_i32_ld_st_disp_12(struct vc4_disas *d, uint16_t *instr)
{
	int32_t o = bf_extract(instr[1],  0, 11);
	int ra    = bf_extract(instr[1], 11, 5);
	int rd    = bf_extract(instr[0],  0, 5);
	int is_st = bf_extract(instr[0],  5, 1);
	int w     = bf_extract(instr[0],  6, 2);
	o        |= bf_extract(instr[0],  8, 1) << 11;
	o = sign_extend(o, 12);

	printf("%s%d r%d, [r%d + %d(0x%04x)]",
		is_st ? "st" : "ld",
		vc4_operand_size(w),
		rd, ra, o, o);
}

void vc4_i_print_i32_ld_st_pre_post(struct vc4_disas *d, uint16_t *instr)
{
	char condstr[32];

	int cond   = bf_extract(instr[1],  7, 4);
	int ra     = bf_extract(instr[1], 11, 5);
	int rd     = bf_extract(instr[0],  0, 5);
	int is_st  = bf_extract(instr[0],  5, 1);
	int w      = bf_extract(instr[0],  6, 2);
	int is_pre = bf_extract(instr[0],  8, 1);

	fill_condstr(condstr, cond);

	printf("%s%d%s r%d, [%sr%d%s]",
		is_st ? "st" : "ld",
		vc4_operand_size(w),
		condstr,
		rd,
		is_pre ? "--" : "",
		ra,
		is_pre ? "" : "++");
}

void vc4_i_print_i32_ld_st_disp_16(struct vc4_disas *d, uint16_t *instr)
{
	int o      = instr[1];
	int rd     = bf_extract(instr[0], 0, 5);
	int is_st  = bf_extract(instr[0], 5, 1);
	int w      = bf_extract(instr[0], 6, 2);
	int r_idx  = bf_extract(instr[0], 8, 2);
	int reg_select[] = { 24, 25, 31, 0 };
	int rs = reg_select[r_idx];
	o = sign_extend(o, 16);

	const char *reg_names[] = { "r25", "sp", "pc", "r0" };

	printf("%s%d r%d, [%s + %d]",
		is_st ? "st" : "ld", vc4_operand_size(w), rd, reg_names[r_idx], o);
	if (rs == get_pc(d, instr))
		print_abs_addr(d, get_pc(d, instr) + o, 0);
}

void vc4_i_print_i32_alu_op(struct vc4_disas *d, uint16_t *instr)
{
	int i = instr[1];
	int rd = bf_extract(instr[0], 0, 5);
	int op = bf_extract(instr[0], 5, 5);
	i = sign_extend(i, 16);
	printf("%s r%d", vc4_scalar_op_get_name(op), rd);
	print_abs_addr(d, i, 1);
}

void vc4_i_print_i32_alu_add(struct vc4_disas *d, uint16_t *instr)
{
	int i = instr[1];
	int rd = bf_extract(instr[0], 0, 5);
	int rs = bf_extract(instr[0], 5, 5);
	i = sign_extend(i, 16);
	printf("add r%d, r%d, 0x%04x", rd, rs, i);
	if (rs == 24)
		print_abs_addr(d, d->data_start + i, 0);
}

void vc4_i_print_i32_alu_add_pc(struct vc4_disas *d, uint16_t *instr)
{
	uint32_t pc;
	int i = instr[1];
	int rd = bf_extract(instr[0], 0, 5);
	i = sign_extend(i, 16);
	printf("add r%d, pc, 0x%04x", rd, i);
	print_abs_addr(d, get_pc(d, instr) + i, 0);
}

void vc4_i_print_i32_alu_cond(struct vc4_disas *d, uint16_t *instr)
{
	char condstr[16];
	char operandstr[16];

	int i    = bf_extract(instr[1],  0, 6);
	int rb   = bf_extract(instr[1],  0, 5);
	int is_i = bf_extract(instr[1],  6, 1);
	int cond = bf_extract(instr[1],  7, 4);
	int ra   = bf_extract(instr[1], 11, 5);
	int rd   = bf_extract(instr[0],  0, 5);
	int op   = bf_extract(instr[0],  5, 6);

	if (is_i) {
		i = sign_extend(i, 16);
		snprintf(operandstr, sizeof(operandstr), "0x%08x", i);
	} else
		snprintf(operandstr, sizeof(operandstr), "r%d", rb);
	fill_condstr(condstr, cond);
	if (op == VC4_SCALAR_OP_MOV)
		printf("%s%s r%d, %s", vc4_scalar_op_get_name(op), condstr, rd, operandstr);
	else if (op == VC4_SCALAR_OP_CMP)
		printf("%s%s r%d, %s", vc4_scalar_op_get_name(op), condstr, ra, operandstr);
	else
		printf("r%d = r%d <%s%s> %s", rd, ra, vc4_scalar_op_get_name(op), condstr, operandstr);
}

void vc4_i_print_instruction_52(struct vc4_disas *d, uint16_t *instr)
{
	char condstr[32];
	uint32_t unsigned1 = ((instr[0] & 0x40) >> 6);
	uint32_t unsigned2 = ((instr[0] & 0x20) >> 5);
	int Rd = (instr[0] & 0x1f);
	int Ra = ((instr[1] & 0xf800) >> 11);
	uint32_t cond = ((instr[1] & 0x780) >> 7);
	int Rb = (instr[1] & 0x1f);
	fill_condstr(condstr, cond);
	printf("mulhd%s r%d, r%d, r%d", condstr, Rd, Ra, Rb);
}

void vc4_i_print_instruction_53(struct vc4_disas *d, uint16_t *instr)
{
	char condstr[32];
	uint32_t unsigned1 = ((instr[0] & 0x40) >> 6);
	uint32_t unsigned2 = ((instr[0] & 0x20) >> 5);
	int Rd = (instr[0] & 0x1f);
	int Ra = ((instr[1] & 0xf800) >> 11);
	uint32_t cond = ((instr[1] & 0x780) >> 7);
	uint32_t imm = (instr[1] & 0x3f);
	fill_condstr(condstr, cond);
	printf("mulhd%s r%d, r%d, 0x%08x", condstr, Rd, Ra, imm);
}

void vc4_i_print_instruction_54(struct vc4_disas *d, uint16_t *instr)
{
	char condstr[32];
	uint32_t unsigned1 = ((instr[0] & 0x40) >> 6);
	uint32_t unsigned2 = ((instr[0] & 0x20) >> 5);
	int Rd = (instr[0] & 0x1f);
	int Ra = ((instr[1] & 0xf800) >> 11);
	uint32_t cond = ((instr[1] & 0x780) >> 7);
	int Rb = (instr[1] & 0x1f);
	fill_condstr(condstr, cond);
	printf("div%s r%d, r%d, r%d", condstr, Rd, Ra, Rb);
}

void vc4_i_print_instruction_55(struct vc4_disas *d, uint16_t *instr)
{
	char condstr[32];
	uint32_t unsigned1 = ((instr[0] & 0x40) >> 6);
	uint32_t unsigned2 = ((instr[0] & 0x20) >> 5);
	int Rd = (instr[0] & 0x1f);
	int Ra = ((instr[1] & 0xf800) >> 11);
	uint32_t cond = ((instr[1] & 0x780) >> 7);
	uint32_t imm = (instr[1] & 0x3f);
	fill_condstr(condstr, cond);
	printf("devxx%s r%d, r%d, r%d", condstr, Rd, Ra, imm);
}

void vc4_i_print_instruction_56(struct vc4_disas *d, uint16_t *instr)
{
	char condstr[32];
	int Rd = (instr[0] & 0x1f);
	int Ra = ((instr[1] & 0xf800) >> 11);
	uint32_t cond = ((instr[1] & 0x780) >> 7);
	int Rb = (instr[1] & 0x1f);
	fill_condstr(condstr, cond);
	printf("addscale%s r%d, r%d, r%d", condstr, Rd, Ra, Rb);
}

void vc4_i_print_i32_alu_fop(struct vc4_disas *d, uint16_t *instr)
{
	char condstr[32];
	uint32_t fop = ((instr[0] & 0x1e0) >> 5);
	int Rd = (instr[0] & 0x1f);
	int Ra = ((instr[1] & 0xf800) >> 11);
	uint32_t cond = ((instr[1] & 0x780) >> 7);
	int Rb = (instr[1] & 0x1f);
	fill_condstr(condstr, cond);
	vc4_float_op_print(fop, Rd, Ra, Rb, condstr);
}

void vc4_i_print_instruction_58(struct vc4_disas *d, uint16_t *instr)
{
	char condstr[32];
	uint32_t fop = ((instr[0] & 0x1e0) >> 5);
	int Rd = (instr[0] & 0x1f);
	int Ra = ((instr[1] & 0xf800) >> 11);
	uint32_t cond = ((instr[1] & 0x780) >> 7);
	uint32_t imm = (instr[1] & 0x3f);
	uint32_t b = 0;
	fill_condstr(condstr, cond);
	vc4_float_op_print_imm(fop, Rd, Ra, imm, condstr);
}

void vc4_i_print_i32_ftrunc(struct vc4_disas *d, uint16_t *instr)
{
	char condstr[32];
	int Rd = (instr[0] & 0x1f);
	int Ra = ((instr[1] & 0xf800) >> 11);
	uint32_t cond = ((instr[1] & 0x780) >> 7);
	uint32_t imm = (instr[1] & 0x3f);
	fill_condstr(condstr, cond);
	printf("ftrunc%s r%d, r%d, sasr 0x%08x", condstr, Rd, Ra, imm);
}

void vc4_i_print_i32_floor(struct vc4_disas *d, uint16_t *instr)
{
	char condstr[32];
	int Rd = (instr[0] & 0x1f);
	int Ra = ((instr[1] & 0xf800) >> 11);
	uint32_t cond = ((instr[1] & 0x780) >> 7);
	uint32_t imm = (instr[1] & 0x3f);
	fill_condstr(condstr, cond);
	printf("floor%s r%d, r%d, sasr 0x%08x", condstr, Rd, Ra, imm);
}

void vc4_i_print_i32_flts(struct vc4_disas *d, uint16_t *instr)
{
	char condstr[32];
	int Rd = (instr[0] & 0x1f);
	int Ra = ((instr[1] & 0xf800) >> 11);
	uint32_t cond = ((instr[1] & 0x780) >> 7);
	uint32_t imm = (instr[1] & 0x3f);
	fill_condstr(condstr, cond);
	printf("flts%s r%d, r%d, sasr 0x%08x", condstr, Rd, Ra, imm);
}

void vc4_i_print_i32_fltu(struct vc4_disas *d, uint16_t *instr)
{
	char condstr[32];
	int Rd = (instr[0] & 0x1f);
	int Ra = ((instr[1] & 0xf800) >> 11);
	uint32_t cond = ((instr[1] & 0x780) >> 7);
	uint32_t imm = (instr[1] & 0x3f);
	fill_condstr(condstr, cond);
	printf("fltu%s r%d, r%d, sasr 0x%08x", condstr, Rd, Ra, imm);
}

static const char *vmemops[] = {
	"ld",           // 00000
	"lookupmh",     // 00001
	"lookupml",     // 00010
	"vop_unknown0", // 00001
	"st",           // 00100
	"indexwritemh", // 00101
	"indexwriteml", // 00110
	"vop_unknown0", // 00111
	"indexwriteml", // 01000
	"indexwriteml", // 01001
};

static const char *const repstr[] = {
	"",
	"x2",
	"x4",
	"x8",
	"x16",
	"x32",
	"x64",
	"x_r0"
};

const char *const pstr[] = {
	"ALL",
	"NONE",
	"IFZ",
	"IFNZ",
	"IFN",
	"IFNN",
	"IFC",
	"IFNC",
};

void vc4_i_print_i48_mem_op(struct vc4_disas *d, uint16_t *instr)
{
	int x = bf_extract(instr[0], 9, 1);
	int mop;
	int w, r;
	uint32_t arg0;
	const char *memstr;

	arg0 = instr[1];
	instr[1] = instr[2];
	instr[2] = arg0;

	arg0 = instr[3];
	instr[3] = instr[4];
	instr[4] = arg0;

	// 1111 00 mop:5 width:2 rs:3 d:10 a:10 z0 111 F:1 rb:6
	// 1111 00mm mmmw wrrr dddd dddd ddaa aaaa aaaa z0 111 F BBBBBB
	// 1111 00mm mmmw wrrr | dddd dddd ddaa aaaa | aaaa z011 1FBB BBBB
	//
	mop = bf_extract(instr[0], 5, 5);
	w = bf_extract(instr[0], 3, 2);
	r = bf_extract(instr[0], 0, 3);
//	d = bf_extract(instr);

	printf("XX%s op: X%d", vmemops[mop], x);
}

void vc4_i_print_i80_mem_op(struct vc4_disas *dis, uint16_t *instr)
{
	int x = bf_extract(instr[0], 9, 1);
	int mop;
	int w, rd, a, rep, d, f, l, f_d, f_a, ra_x, p, i, rs, imm;
	uint32_t arg0, arg1;
	const char *memstr;
	char buf[64];

	arg0 = (instr[1] << 16) | instr[2];
	arg1 = (instr[3] << 16) | instr[4];

	printf("vmem:");
	/*
	 * 1111 10 mop:5 width:2 r:3 | 1110 rd:6 a:10 F 0 111 l:7 | f_d:6 f_a:6 Ra_x:4 P:3   i:7 rs:4 i:2
	 * 1111 10 mop:5 width:2 r:3 | d:10 1110 ra:6 F 0 111 l:7 | f_d:6 f_a:6 Ra_x:4 P:3   i:7 rs:4 i:2
	 * 1111 10 mop:5 width:2 r:3 | d:10 a:10      F 0 b:10    | f_d:6 f_a:6 Ra_x:4 P:3 f_i:7    f_b:6
	 * 1111 10 mop:5 width:2 r:3 | d:10 a:10      F 1 l:10    | f_d:6 f_a:6 Ra_x:4 P:3 f_i:7      j:6
	*/
	if (bf_extract(arg0, 28, 4) == 0b1110) {
		if (bf_extract(arg0, 7, 4) != 0b0111)
			printf("0:");
		else
			printf("+:");
	}

	mop  = bf_extract(instr[0], 5, 5);
	w    = bf_extract(instr[0], 3, 2);
	rep  = bf_extract(instr[0], 0, 3);
	d    = bf_extract(arg0, 22, 10);
	rd   = bf_extract(arg0, 22, 6);
	a    = bf_extract(arg0, 12, 10);
	l    = bf_extract(arg0, 0, 7);
	i    = bf_extract(arg1, 0, 2) | (bf_extract(arg1, 6, 7) << 2);
	rs   = bf_extract(arg1, 2, 4);
	ra_x = bf_extract(arg1, 16, 4);
	printf("%s%d", vmemops[mop], w * 2);
	if (bf_extract(arg0, 28, 4) == 0b1110) {
		imm = l | (i << 7);
		i80_arg_to_str(buf, sizeof(buf), a, ra_x);
		printf(" -, %s 0x%04x(r%d+=r%d)", buf, imm, rs, rd);
	} else if (bf_extract(arg0, 24, 4)) {
		printf("-1");
	} else if (!bf_test(arg0, 10)) {
		i80_arg_to_str(buf, sizeof(buf), d, -1);
		printf("%s", buf);
		printf("-2");
	} else {
		i80_arg_to_str(buf, sizeof(buf), d, -1);
		printf("%s", buf);
		printf("-3");
	}

	if (rep)
		printf(" %s", repstr[rep]);
}

#define DECL_VOP_48(idx, op) [idx] = #op
const char *vop_48_string[] = {
	DECL_VOP_48(0b000000, vmov),
	DECL_VOP_48(0b000001, vbitplanes),
	DECL_VOP_48(0b000010, veven),
	DECL_VOP_48(0b000011, vodd),
	DECL_VOP_48(0b000100, vinterl),
	DECL_VOP_48(0b000101, vinterh),
	DECL_VOP_48(0b000000, vmov),
	DECL_VOP_48(0b000000, vmov),
	DECL_VOP_48(0b000110, vbitrev),
	DECL_VOP_48(0b000111, vror),
	DECL_VOP_48(0b001000, vshl),
	DECL_VOP_48(0b001001, vshls),
	DECL_VOP_48(0b001010, vlsr),
	DECL_VOP_48(0b001011, vasr),
	DECL_VOP_48(0b001100, vsignshl),
	DECL_VOP_48(0b001101, vop13),
	DECL_VOP_48(0b001110, vsignasl),
	DECL_VOP_48(0b001111, vsignasls),
	DECL_VOP_48(0b010000, vand),
	DECL_VOP_48(0b010001, vor),
	DECL_VOP_48(0b010010, veor),
	DECL_VOP_48(0b010011, vbic),
	DECL_VOP_48(0b010100, vcount),
	DECL_VOP_48(0b010101, vmsb),
	DECL_VOP_48(0b010110, vop22),
	DECL_VOP_48(0b010111, vop23),
	DECL_VOP_48(0b011000, vmin),
	DECL_VOP_48(0b011001, vmax),
	DECL_VOP_48(0b011010, vdist),
	DECL_VOP_48(0b011011, vdists),
	DECL_VOP_48(0b011100, vclip),
	DECL_VOP_48(0b011101, vsign),
	DECL_VOP_48(0b011110, vclips),
	DECL_VOP_48(0b011111, vtestmag),
	DECL_VOP_48(0b100000, vadd),
	DECL_VOP_48(0b100001, vadds),
	DECL_VOP_48(0b100010, vaddc),
	DECL_VOP_48(0b100011, vaddsc),
	DECL_VOP_48(0b100100, vsub),
	DECL_VOP_48(0b100101, vsubs),
	DECL_VOP_48(0b100110, vsubc),
	DECL_VOP_48(0b100111, vsubsc),
	DECL_VOP_48(0b101000, vrsub),
	DECL_VOP_48(0b101001, vrsubs),
	DECL_VOP_48(0b101010, vrsubc),
	DECL_VOP_48(0b101011, vrsubsc),
	DECL_VOP_48(0b101100, vop44),
	DECL_VOP_48(0b101101, vop45),
	DECL_VOP_48(0b101110, vop46),
	DECL_VOP_48(0b101111, vop47)
};

void vc4_i_print_i48_data_vop(struct vc4_disas *dis, uint16_t *instr)
{
	const char *vopstr;
	/*
	 * v<op><X> H(yd,xd)[+rs], H(ya,xa)[+rs], (rb) [SETF]
	 * v<op><X> V(yd,xd)[+rs], V(ya,xa)[+rs], (rb) [SETF]
	 * v<op><X> H(yd,xd)[+rs], H(ya,xa)[+rs], H(yb,xb)[+rs]
	 * v<op><X> V(yd,xd)[+rs], V(ya,xa)[+rs], H(yb,xb)[+rs]
	 * v<op><X> H(yd,xd)[+rs], H(ya,xa)[+rs], #immediate  [SETF] [IFZ|IFNZ|IFN|IFNN|IFC|IFNC]
	 * v<op><X> V(yd,xd)[+rs], V(ya,xa)[+rs], #immediate  [SETF] [IFZ|IFNZ|IFN|IFNN|IFC|IFNC]
	 */

	/*
	 * 1111 01 X vop:6 rs:3 | d:10 a:10 z0 111 F rb:6   v<op><X> Rd[+rs], Ra[+rs], (rb) [SETF]
	 * 1111 01 X vop:6 rs:3 | d:10 a:10 z0 b:10         v<op><X> Rd[+rs], Ra[+rs], Rb[+rs]
	 * 1111 01 X vop:6 rs:3 | d:10 a:10 z1 P:3 F:1 i:6  v<op><X> Rd[+rs], Ra[+rs], #i [SETF] [IFZ|IFNZ|IFN|IFNN|IFC|IFNC]
	 */
	int x, vop, rs, d, a, z, f, rb, b, P, i;
	uint32_t arg;
	char buf[64];

	arg = (instr[1] << 16) | instr[2];
	x   = bf_extract(instr[0], 9, 1);
	vop = bf_extract(instr[0], 3, 6);
	rs  = bf_extract(instr[0], 0, 3);
	d   = bf_extract(arg, 22, 10);
	a   = bf_extract(arg, 12, 10);
	z   = bf_extract(arg, 11, 1);
	f   = bf_extract(arg, 6, 1);
	rb  = bf_extract(arg, 0, 6);
	vopstr = vop_48_string[vop];

	printf("%s.%d", vopstr, x);

	i80_arg_to_str(buf, sizeof(buf), d, -1);
	printf(" %s", buf);
	i80_arg_to_str(buf, sizeof(buf), a, -1);
	printf(", %s", buf);
	if (bf_extract(arg, 7, 4) == 0b0111) {
		printf(", (r%d)", rb);
	} else if (!bf_test(arg, 10)) {
		i80_arg_to_str(buf, sizeof(buf), b, -1);
		printf(", %s", buf);
	} else
		printf(", %d", rb);
	// printf("data_vop:%d,%d", x, vop);
}

void vc4_i_print_i80_data_op(struct vc4_disas *dis, uint16_t *instr)
{
	const char *vopstr = "<VOPSTR>";
	int d, a, b, ra_x, m, f_d, f_a, f_b;
	int opx, rep, p, vop, f_i;

	char argstr_dest[32];
	char argstr_src0[32];
	char argstr_src1[32];

	uint32_t arg0;
	uint32_t arg1;
	int imm;

	/*
	 * 1111 11 X v:6 r:3 | d:10 a:10 F 0 b:10 | f_d:6 f_a:6 Ra_x:4 P:3 f_i:7 f_b:6
	 * 1111 11 X v:6 r:3 | d:10 a:10 F 1 k:10 | f_d:6 f_a:6 Ra_x:4 P:3 f_i:7   j:6
	 *
	 */

	arg0 = (instr[1] << 16) | instr[2];
	arg1 = (instr[3] << 16) | instr[4];

	opx = bf_extract(instr[0], 9, 1);
	vop = bf_extract(instr[0], 3, 6);
	rep = bf_extract(instr[0], 0, 3);

	b = bf_extract(arg0,  0, 10);
	m = bf_extract(arg0, 10,  1);
	a = bf_extract(arg0, 12, 10);
	d = bf_extract(arg0, 22, 10);

	f_d  = bf_extract(arg1, 26, 6);
	f_a  = bf_extract(arg1, 20, 6);
	ra_x = bf_extract(arg1, 16, 4);
	p    = bf_extract(arg1, 13, 3);
	f_i  = bf_extract(arg1,  6, 7);
	f_b  = bf_extract(arg1,  0, 6);

	if (vop <= 48)
		vopstr = vop_48_string[vop];

	i80_arg_to_str(argstr_dest, sizeof(argstr_dest), d,   -1);
	i80_arg_to_str(argstr_src0, sizeof(argstr_src0), a, ra_x);

#if 0
	printf("vop:%s:", m ? "m" : "");
	printf("0x%08x 0x%08x,", arg0, arg1);
#endif

	if (m) {
		imm = b | (f_b << 6);
		printf("%s %s, %s, 0x%08x, fi:0x%0x, %s, %s",
			vopstr,
			argstr_dest,
			argstr_src0,
			imm,
			f_i,
			pstr[p],
			repstr[rep]);
	} else {
		i80_arg_to_str(argstr_src1, sizeof(argstr_src1), b, -1);
		printf("%s %s, %s, %s, %d, %s, %s",
			vopstr,
			argstr_dest,
			argstr_src0,
			argstr_src1,
			f_i,
			pstr[p],
			repstr[rep]);
	}
}

void vc4_i_print_i80_stub(struct vc4_disas *d, uint16_t *instr)
{
	printf("i80_stub");
}
