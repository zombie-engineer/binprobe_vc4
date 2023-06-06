#include "vc4_scalar_op.h"
#include <stdio.h>

VC4_SCALAR_OP_PRINT_FN(mov)
{
	printf("mov%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(cmn)
{
	printf("cmn%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(add)
{
	printf("add%s r%d, r%d, r%d", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(bic)
{
	printf("bic%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(mul)
{
	printf("mul%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(eor)
{
	printf("eor%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(sub)
{
	printf("sub%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(and)
{
	printf("and%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(not)
{
	printf("not%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(ror)
{
	printf("ror%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(cmp)
{
	printf("cmp r%d, 0x%08x, 0x%08x", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(rsb)
{
	printf("rsb%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(btst)
{
	printf("btst r%d, %d", a, b);
}

VC4_SCALAR_OP_PRINT_FN(or)
{
	printf("or%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(bmask)
{
	printf("bmask%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(max)
{
	printf("max%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(bset)
{
	printf("bset%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(min)
{
	printf("min%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(bclr)
{
	printf("bclr%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(adds2)
{
	printf("adds%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(bchg)
{
	printf("bchg%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(adds4)
{
	printf("adds4%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(adds8)
{
	printf("adds8%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(adds16)
{
	printf("adds16%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(exts)
{
	printf("exts%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(neg)
{
	printf("exts%s r%d, 0x%08x", cond, d, b);
}

VC4_SCALAR_OP_PRINT_FN(lsr)
{
	printf("exts%s r%d, 0x%08x, 0x%08x", cond, d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(clz)
{
	printf("clz");
}

VC4_SCALAR_OP_PRINT_FN(lsl)
{
	printf("lsl r%d, 0x%08x, 0x%08x", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(brev)
{
	printf("brev r%d, 0x%08x, 0x%08x", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(asr)
{
	printf("asr r%d, 0x%08x, 0x%08x", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(abs)
{
	printf("abs r%d, 0x%08x, 0x%08x", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(mulhd_ss)
{
	printf("mulhd.ss r%d, (r%d * r%d) >> 32", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(mulhd_su)
{
	printf("mulhd.su r%d, (r%d * r%d) >> 32", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(mulhd_us)
{
	printf("mulhd.us r%d, (r%d * r%d) >> 32", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(mulhd_uu)
{
	printf("mulhd.uu r%d, (r%d * r%d) >> 32", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(div_ss)
{
	printf("div.ss r%d, (r%d / r%d)", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(div_su)
{
	printf("div.su r%d, (r%d / r%d)", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(div_us)
{
	printf("div.us r%d, (r%d / r%d)", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(div_uu)
{
	printf("div.uu r%d, (r%d / r%d)", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(adds)
{
	printf("adds r%d, r%d, r%d", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(subs)
{
	printf("subs r%d, r%d, r%d", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(shls)
{
	printf("shls r%d, r%d, r%d", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(clipsh)
{
	printf("clipsh r%d, r%d", d, b);
}

VC4_SCALAR_OP_PRINT_FN(addscale5)
{
	printf("addscale r%d, r%d, r%d << 5", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(addscale6)
{
	printf("addscale r%d, r%d, r%d << 6", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(addscale7)
{
	printf("addscale r%d, r%d, r%d << 7", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(addscale8)
{
	printf("addscale r%d, r%d, r%d << 8", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(count)
{
	printf("count r%d, r%d", d, b);
}

VC4_SCALAR_OP_PRINT_FN(subscale1)
{
	printf("subscale r%d, r%d, r%d << 1", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(subscale2)
{
	printf("subscale r%d, r%d, r%d << 2", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(subscale3)
{
	printf("subscale r%d, r%d, r%d << 3", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(subscale4)
{
	printf("subscale r%d, r%d, r%d << 4", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(subscale5)
{
	printf("subscale r%d, r%d, r%d << 5", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(subscale6)
{
	printf("subscale r%d, r%d, r%d << 6", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(subscale7)
{
	printf("subscale r%d, r%d, r%d << 7", d, a, b);
}

VC4_SCALAR_OP_PRINT_FN(subscale8)
{
	printf("subscale r%d, r%d, r%d << 8", d, a, b);
}

#if 0
#define DECL_OP VC4_SCALAR_OP_INITIALIZER

struct vc4_scalar_op vc4_scalar_op_table[] = {
	DECL_OP(MOV),
	DECL_OP(CMN),
	DECL_OP(ADD),
	DECL_OP(BIC),
	DECL_OP(MUL),
	DECL_OP(EOR),
	DECL_OP(SUB),
	DECL_OP(AND),
	DECL_OP(NOT),
	DECL_OP(ROR),
	DECL_OP(CMP),
	DECL_OP(RSB),
	DECL_OP(BTST),
	DECL_OP(OR),
	DECL_OP(BMASK),
	DECL_OP(MAX),
	DECL_OP(BSET),
	DECL_OP(MIN),
	DECL_OP(BCLR),
	DECL_OP(ADDS2),
	DECL_OP(BCHG),
	DECL_OP(ADDS4),
	DECL_OP(ADDS8),
	DECL_OP(ADDS16),
	DECL_OP(EXTS),
	DECL_OP(NEG),
	DECL_OP(LSR),
	DECL_OP(CLZ),
	DECL_OP(LSL),
	DECL_OP(BREV),
	DECL_OP(ASR),
	DECL_OP(ABS),
	DECL_OP(MULHD_SS),
	DECL_OP(MULHD_SU),
	DECL_OP(MULHD_US),
	DECL_OP(MULHD_UU),
	DECL_OP(DIV_SS),
	DECL_OP(DIV_SU),
	DECL_OP(DIV_US),
	DECL_OP(DIV_UU),
	DECL_OP(ADDS),
	DECL_OP(SUBS),
	DECL_OP(SHLS),
	DECL_OP(CLIPSH),
	DECL_OP(ADDSCALE5),
	DECL_OP(ADDSCALE6),
	DECL_OP(ADDSCALE7),
	DECL_OP(ADDSCALE8),
	DECL_OP(COUNT),
	DECL_OP(SUBSCALE1),
	DECL_OP(SUBSCALE2),
	DECL_OP(SUBSCALE3),
	DECL_OP(SUBSCALE4),
	DECL_OP(SUBSCALE5),
	DECL_OP(SUBSCALE6),
	DECL_OP(SUBSCALE7),
	DECL_OP(SUBSCALE8),
};
#undef DECL_OP

#endif
void vc4_scalar_op_init(void)
{
}

const char *vc4_scalar_op_get_name(int op)
{
	return vc4_get_scalar_op(op)->name;
}
