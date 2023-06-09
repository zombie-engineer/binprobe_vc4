/* autogenerated file */
#include "vc4_scalar_op.h"

VC4_SCALAR_OP_FNS(mov);
VC4_SCALAR_OP_FNS(cmn);
VC4_SCALAR_OP_FNS(add);
VC4_SCALAR_OP_FNS(bic);
VC4_SCALAR_OP_FNS(mul);
VC4_SCALAR_OP_FNS(eor);
VC4_SCALAR_OP_FNS(sub);
VC4_SCALAR_OP_FNS(and);
VC4_SCALAR_OP_FNS(not);
VC4_SCALAR_OP_FNS(ror);
VC4_SCALAR_OP_FNS(cmp);
VC4_SCALAR_OP_FNS(rsb);
VC4_SCALAR_OP_FNS(btst);
VC4_SCALAR_OP_FNS(or);
VC4_SCALAR_OP_FNS(bmask);
VC4_SCALAR_OP_FNS(max);
VC4_SCALAR_OP_FNS(bset);
VC4_SCALAR_OP_FNS(min);
VC4_SCALAR_OP_FNS(bclr);
VC4_SCALAR_OP_FNS(adds2);
VC4_SCALAR_OP_FNS(bchg);
VC4_SCALAR_OP_FNS(adds4);
VC4_SCALAR_OP_FNS(adds8);
VC4_SCALAR_OP_FNS(adds16);
VC4_SCALAR_OP_FNS(exts);
VC4_SCALAR_OP_FNS(neg);
VC4_SCALAR_OP_FNS(lsr);
VC4_SCALAR_OP_FNS(clz);
VC4_SCALAR_OP_FNS(lsl);
VC4_SCALAR_OP_FNS(brev);
VC4_SCALAR_OP_FNS(asr);
VC4_SCALAR_OP_FNS(abs);
VC4_SCALAR_OP_FNS(mulhd_ss);
VC4_SCALAR_OP_FNS(mulhd_su);
VC4_SCALAR_OP_FNS(mulhd_us);
VC4_SCALAR_OP_FNS(mulhd_uu);
VC4_SCALAR_OP_FNS(div_ss);
VC4_SCALAR_OP_FNS(div_su);
VC4_SCALAR_OP_FNS(div_us);
VC4_SCALAR_OP_FNS(div_uu);
VC4_SCALAR_OP_FNS(adds);
VC4_SCALAR_OP_FNS(subs);
VC4_SCALAR_OP_FNS(shls);
VC4_SCALAR_OP_FNS(clipsh);
VC4_SCALAR_OP_FNS(addscale5);
VC4_SCALAR_OP_FNS(addscale6);
VC4_SCALAR_OP_FNS(addscale7);
VC4_SCALAR_OP_FNS(addscale8);
VC4_SCALAR_OP_FNS(count);
VC4_SCALAR_OP_FNS(subscale1);
VC4_SCALAR_OP_FNS(subscale2);
VC4_SCALAR_OP_FNS(subscale3);
VC4_SCALAR_OP_FNS(subscale4);
VC4_SCALAR_OP_FNS(subscale5);
VC4_SCALAR_OP_FNS(subscale6);
VC4_SCALAR_OP_FNS(subscale7);
VC4_SCALAR_OP_FNS(subscale8);

static struct vc4_scalar_op vc4_scalar_op_table[] = {
	VC4_SCALAR_OP_TABLE_ENTRY(mov),
	VC4_SCALAR_OP_TABLE_ENTRY(cmn),
	VC4_SCALAR_OP_TABLE_ENTRY(add),
	VC4_SCALAR_OP_TABLE_ENTRY(bic),
	VC4_SCALAR_OP_TABLE_ENTRY(mul),
	VC4_SCALAR_OP_TABLE_ENTRY(eor),
	VC4_SCALAR_OP_TABLE_ENTRY(sub),
	VC4_SCALAR_OP_TABLE_ENTRY(and),
	VC4_SCALAR_OP_TABLE_ENTRY(not),
	VC4_SCALAR_OP_TABLE_ENTRY(ror),
	VC4_SCALAR_OP_TABLE_ENTRY(cmp),
	VC4_SCALAR_OP_TABLE_ENTRY(rsb),
	VC4_SCALAR_OP_TABLE_ENTRY(btst),
	VC4_SCALAR_OP_TABLE_ENTRY(or),
	VC4_SCALAR_OP_TABLE_ENTRY(bmask),
	VC4_SCALAR_OP_TABLE_ENTRY(max),
	VC4_SCALAR_OP_TABLE_ENTRY(bset),
	VC4_SCALAR_OP_TABLE_ENTRY(min),
	VC4_SCALAR_OP_TABLE_ENTRY(bclr),
	VC4_SCALAR_OP_TABLE_ENTRY(adds2),
	VC4_SCALAR_OP_TABLE_ENTRY(bchg),
	VC4_SCALAR_OP_TABLE_ENTRY(adds4),
	VC4_SCALAR_OP_TABLE_ENTRY(adds8),
	VC4_SCALAR_OP_TABLE_ENTRY(adds16),
	VC4_SCALAR_OP_TABLE_ENTRY(exts),
	VC4_SCALAR_OP_TABLE_ENTRY(neg),
	VC4_SCALAR_OP_TABLE_ENTRY(lsr),
	VC4_SCALAR_OP_TABLE_ENTRY(clz),
	VC4_SCALAR_OP_TABLE_ENTRY(lsl),
	VC4_SCALAR_OP_TABLE_ENTRY(brev),
	VC4_SCALAR_OP_TABLE_ENTRY(asr),
	VC4_SCALAR_OP_TABLE_ENTRY(abs),
	VC4_SCALAR_OP_TABLE_ENTRY(mulhd_ss),
	VC4_SCALAR_OP_TABLE_ENTRY(mulhd_su),
	VC4_SCALAR_OP_TABLE_ENTRY(mulhd_us),
	VC4_SCALAR_OP_TABLE_ENTRY(mulhd_uu),
	VC4_SCALAR_OP_TABLE_ENTRY(div_ss),
	VC4_SCALAR_OP_TABLE_ENTRY(div_su),
	VC4_SCALAR_OP_TABLE_ENTRY(div_us),
	VC4_SCALAR_OP_TABLE_ENTRY(div_uu),
	VC4_SCALAR_OP_TABLE_ENTRY(adds),
	VC4_SCALAR_OP_TABLE_ENTRY(subs),
	VC4_SCALAR_OP_TABLE_ENTRY(shls),
	VC4_SCALAR_OP_TABLE_ENTRY(clipsh),
	VC4_SCALAR_OP_TABLE_ENTRY(addscale5),
	VC4_SCALAR_OP_TABLE_ENTRY(addscale6),
	VC4_SCALAR_OP_TABLE_ENTRY(addscale7),
	VC4_SCALAR_OP_TABLE_ENTRY(addscale8),
	VC4_SCALAR_OP_TABLE_ENTRY(count),
	VC4_SCALAR_OP_TABLE_ENTRY(subscale1),
	VC4_SCALAR_OP_TABLE_ENTRY(subscale2),
	VC4_SCALAR_OP_TABLE_ENTRY(subscale3),
	VC4_SCALAR_OP_TABLE_ENTRY(subscale4),
	VC4_SCALAR_OP_TABLE_ENTRY(subscale5),
	VC4_SCALAR_OP_TABLE_ENTRY(subscale6),
	VC4_SCALAR_OP_TABLE_ENTRY(subscale7),
	VC4_SCALAR_OP_TABLE_ENTRY(subscale8)
};

const struct vc4_scalar_op *vc4_get_scalar_op(int op_idx)
{
	return &vc4_scalar_op_table[op_idx];
}
