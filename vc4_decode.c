#include "vc4_decode.h"
#include "bitfield.h"
#include <stddef.h>
#include <stdint.h>
#include "vc4_instructions.h"

#define VC4_DISAS_STRUCT(__a) vc4_disas_i_ ## __a

#define VC4_DISAS_DECL(__a) \
  struct vc4_disas_instruction VC4_DISAS_STRUCT(__a) = { \
   .print = vc4_i_print_ ## __a \
  }

VC4_DISAS_DECL(i16_illegal);
VC4_DISAS_DECL(i16_stub);
VC4_DISAS_DECL(i16_add_sp);
VC4_DISAS_DECL(i16_bkpt);
VC4_DISAS_DECL(i16_nop);
VC4_DISAS_DECL(i16_sleep);
VC4_DISAS_DECL(i16_user);
VC4_DISAS_DECL(i16_ei);
VC4_DISAS_DECL(i16_di);
VC4_DISAS_DECL(i16_cbclr);
VC4_DISAS_DECL(i16_cbadd1);
VC4_DISAS_DECL(i16_cbadd2);
VC4_DISAS_DECL(i16_cbadd3);
VC4_DISAS_DECL(i16_rti);
VC4_DISAS_DECL(i16_swi);
VC4_DISAS_DECL(i16_b);
VC4_DISAS_DECL(i16_switch);
VC4_DISAS_DECL(i16_version);
VC4_DISAS_DECL(i16_swi_u);
VC4_DISAS_DECL(i16_ldm_stm);
VC4_DISAS_DECL(i16_ld_st_sp);
VC4_DISAS_DECL(i16_ldw_stw);
VC4_DISAS_DECL(i16_b_cond);
VC4_DISAS_DECL(i16_ld_st);
VC4_DISAS_DECL(i16_alu_op);
VC4_DISAS_DECL(i32_p_control);
VC4_DISAS_DECL(i32_stub);
VC4_DISAS_DECL(i32_addcmpb);
VC4_DISAS_DECL(i32_b);
VC4_DISAS_DECL(i32_ld_st_cond_indexed);
VC4_DISAS_DECL(i32_ld_st_disp_12);
VC4_DISAS_DECL(i32_ld_st_pre_post);
VC4_DISAS_DECL(i32_ld_st_disp_16);
VC4_DISAS_DECL(i32_alu_op);
VC4_DISAS_DECL(i32_alu_add);
VC4_DISAS_DECL(i32_alu_add_pc);
VC4_DISAS_DECL(i32_alu_cond);
VC4_DISAS_DECL(i32_alu_fop);
VC4_DISAS_DECL(i32_ftrunc);
VC4_DISAS_DECL(i32_floor);
VC4_DISAS_DECL(i32_flts);
VC4_DISAS_DECL(i32_fltu);
VC4_DISAS_DECL(i48_b);
VC4_DISAS_DECL(i48_add_pc_rel);
VC4_DISAS_DECL(i48_add_ld_st);
VC4_DISAS_DECL(i48_alu_op);
VC4_DISAS_DECL(i48_alu_add);
VC4_DISAS_DECL(i48_stub);
VC4_DISAS_DECL(i48_mem_op);
VC4_DISAS_DECL(i48_data_vop);
VC4_DISAS_DECL(i80_mem_op);
VC4_DISAS_DECL(i80_data_op);
VC4_DISAS_DECL(i80_stub);
VC4_DISAS_DECL(instruction_52);
VC4_DISAS_DECL(instruction_53);
VC4_DISAS_DECL(instruction_54);
VC4_DISAS_DECL(instruction_55);
VC4_DISAS_DECL(instruction_56);
VC4_DISAS_DECL(instruction_58);


const struct vc4_disas_instruction *vc4_decode_instr16(uint16_t i)
{
	if ((i & 0xfff0) == 0x0000) {
		switch(i) {
			case  0: return &VC4_DISAS_STRUCT(i16_bkpt);
			case  1: return &VC4_DISAS_STRUCT(i16_nop);
			case  2: return &VC4_DISAS_STRUCT(i16_sleep);
			case  3: return &VC4_DISAS_STRUCT(i16_user);
			case  4: return &VC4_DISAS_STRUCT(i16_ei);
			case  5: return &VC4_DISAS_STRUCT(i16_di);
			case  6: return &VC4_DISAS_STRUCT(i16_cbclr);
			case  7: return &VC4_DISAS_STRUCT(i16_cbadd1);
			case  8: return &VC4_DISAS_STRUCT(i16_cbadd1);
			case  9: return &VC4_DISAS_STRUCT(i16_cbadd1);
			case 10: return &VC4_DISAS_STRUCT(i16_rti);
			default: return &VC4_DISAS_STRUCT(i16_illegal);
		}
	}

	if ((i & 0xffe0) == 0x0020)
		return &VC4_DISAS_STRUCT(i16_swi);

	if ((i & 0xffc0) == 0x0040)
		return &VC4_DISAS_STRUCT(i16_b);

	if ((i & 0xffd0) == 0x0080)
		return &VC4_DISAS_STRUCT(i16_switch);

	if ((i & 0xffe0) == 0x00e0)
		return &VC4_DISAS_STRUCT(i16_version);

	if ((i & 0xffc0) == 0x01c0)
		return &VC4_DISAS_STRUCT(i16_swi_u);

	if ((i & 0xfe00) == 0x0200)
		return &VC4_DISAS_STRUCT(i16_ldm_stm);

	if ((i & 0xfc00) == 0x0400)
		return &VC4_DISAS_STRUCT(i16_ld_st_sp);

	if ((i & 0xf800) == 0x0800)
		return &VC4_DISAS_STRUCT(i16_ldw_stw);

	if ((i & 0xf800) == 0x1000)
		return &VC4_DISAS_STRUCT(i16_add_sp);

	if ((i & 0xf800) == 0x1800)
		return &VC4_DISAS_STRUCT(i16_b_cond);

	if ((i & 0xe000) == 0x2000)
		return &VC4_DISAS_STRUCT(i16_ld_st);

	if ((i & 0xc000) == 0x4000)
		return &VC4_DISAS_STRUCT(i16_alu_op);

	return NULL;
}

const struct vc4_disas_instruction *vc4_decode_instr32(uint32_t i)
{
	if ((i & 0xf0000000) == 0x80000000)
		return &VC4_DISAS_STRUCT(i32_addcmpb);

	if ((i & 0xf0000000) == 0x90000000)
		return &VC4_DISAS_STRUCT(i32_b);

	if ((i & 0xff000060) == 0xa0000000)
		return &VC4_DISAS_STRUCT(i32_ld_st_cond_indexed);

	if ((i & 0xfe000000) == 0xa2000000)
		return &VC4_DISAS_STRUCT(i32_ld_st_disp_12);

	if ((i & 0xfe00007f) == 0xa4000000)
		return &VC4_DISAS_STRUCT(i32_ld_st_pre_post);

	if ((i & 0xfc000000) == 0xa8000000)
		return &VC4_DISAS_STRUCT(i32_ld_st_disp_16);

	if ((i & 0xfc000000) == 0xb0000000)
		return &VC4_DISAS_STRUCT(i32_alu_op);

	if ((i & 0xfc000000) == 0xb4000000)
		return &VC4_DISAS_STRUCT(i32_alu_add);

	if ((i & 0xffe00000) == 0xbfe00000)
		return &VC4_DISAS_STRUCT(i32_alu_add_pc);

	if (((i & 0xf8000060) == 0xc0000000) || ((i & 0xf8000040) == 0xc0000040))
		return &VC4_DISAS_STRUCT(i32_alu_cond);

	if (((i & 0xfe000060) == 0xc8000000) || ((i & 0xfe000040) == 0xc8000040))
		return &VC4_DISAS_STRUCT(i32_alu_fop);

	if ((i & 0xff800060) == 0xca000000 || ((i & 0xff800040) == 0xca000040)) {
		const struct vc4_disas_instruction const *f_ops[] = {
			&VC4_DISAS_STRUCT(i32_ftrunc),
			&VC4_DISAS_STRUCT(i32_floor),
			&VC4_DISAS_STRUCT(i32_flts),
			&VC4_DISAS_STRUCT(i32_fltu)
		};
		int f_op_index = bf_extract(i, 21, 2);
		return f_ops[f_op_index];
	}

	if ((i & 0xffc0ffe0) == 0xcc000000)
		return &VC4_DISAS_STRUCT(i32_p_control);

	return NULL;
}

const struct vc4_disas_instruction *vc4_decode_instr48(uint16_t i, uint32_t i32)
{
	/*
	 * 1110 10 p:5 d:5 u:32
	 * 1110 11 s:5 d:5 u:32
	 */
	if ((i & 0xfc00) == 0xf400)
		return &VC4_DISAS_STRUCT(i48_data_vop);

	if ((i & 0xfc00) == 0xe000)
		return &VC4_DISAS_STRUCT(i48_b);

	if ((i & 0xff00) == 0xe500)
		return &VC4_DISAS_STRUCT(i48_add_pc_rel);

	if ((i & 0xfe00) == 0xe600)
		return &VC4_DISAS_STRUCT(i48_add_ld_st);

	if ((i & 0xfc00) == 0xe800)
		return &VC4_DISAS_STRUCT(i48_alu_op);

	if ((i & 0xfc00) == 0xec00)
		return &VC4_DISAS_STRUCT(i48_alu_add);

	return NULL;
}

const struct vc4_disas_instruction *vc4_decode_instr80(uint16_t *i)
{
	if ((i[0] & 0xfc00) == 0xf800)
		return &VC4_DISAS_STRUCT(i80_mem_op);

	if ((i[0] & 0xfc00) == 0xfc00)
		return &VC4_DISAS_STRUCT(i80_data_op);
	return NULL;
}

const struct vc4_disas_instruction *vc4_decode_instr(uint16_t *i, int i_sz)
{
	const struct vc4_disas_instruction *found = NULL;

	int i_width_bits = bf_extract(i[0], 13, 3);

	if ((i[0] & 0x8000) == 0)
		return vc4_decode_instr16(i[0]);

	if ((i[0] & 0xfc00) == 0xf400)
		return vc4_decode_instr48(i[0], (i[2] << 16)|i[1]);

	if ((i[0] & 0xf800) == 0xf800)
		return vc4_decode_instr80(i);

	if ((i[0] & 0xe000) == 0xe000)
		return vc4_decode_instr48(i[0], (i[2] << 16)|i[1]);

	return vc4_decode_instr32((i[0] << 16) | i[1]);
}
