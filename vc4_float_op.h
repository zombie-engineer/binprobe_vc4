#pragma once
#include <stdio.h>

#define VC4_FLOAT_PRINT_FN(__op) \
  void vc4_float_op_print_ ## __op(int d, int a, int b, const char *cond)

#define VC4_FLOAT_PRINT_IMM_FN(__op) \
  void vc4_float_op_print_imm_ ## __op(int d, int a, uint32_t b,\
    const char *cond)

#define VC4_FLOAT_PRINT(__op, __print, ...) \
  static VC4_FLOAT_PRINT_FN(__op) { printf(__print, ## __VA_ARGS__); }

#define VC4_FLOAT_PRINT_IMM(__op, __print, ...) \
  static VC4_FLOAT_PRINT_IMM_FN(__op) { printf(__print, ## __VA_ARGS__); }

VC4_FLOAT_PRINT(fadd, "fadd%s r%d, r%d, r%d", cond, d, a, b);
VC4_FLOAT_PRINT(fsub, "fsub%s r%d, r%d, r%d", cond, d, a, b);
VC4_FLOAT_PRINT(fmul, "fmul%s r%d, r%d, r%d", cond, d, a, b);
VC4_FLOAT_PRINT(fdiv, "fdiv%s", cond);
VC4_FLOAT_PRINT(fcmp, "fcmp%s", cond);
VC4_FLOAT_PRINT(fabs, "fabs%s", cond);
VC4_FLOAT_PRINT(frsb, "frsb%s", cond);
VC4_FLOAT_PRINT(fmax, "fmax%s", cond);
VC4_FLOAT_PRINT(frcp, "frcp%s", cond);
VC4_FLOAT_PRINT(frsqrt, "frsqrt%s", cond);
VC4_FLOAT_PRINT(ffnmul, "ffnmul%s", cond);
VC4_FLOAT_PRINT(fnmul, "fnmul%s", cond);
VC4_FLOAT_PRINT(fmin, "fmin%s", cond);
VC4_FLOAT_PRINT(fld0, "fld0%s", cond);
VC4_FLOAT_PRINT(fld1, "fld1%s", cond);
VC4_FLOAT_PRINT(flog2, "flog2%s", cond);
VC4_FLOAT_PRINT(fexp2, "fexp2%s", cond);

VC4_FLOAT_PRINT_IMM(fadd, "fadd%s r%d, r%d, 0x%08x", cond, d, a, b);
VC4_FLOAT_PRINT_IMM(fsub, "fsub%s r%d, r%d, 0x%08x", cond, d, a, b);
VC4_FLOAT_PRINT_IMM(fmul, "fmul%s r%d, r%d, 0x%08x", cond, d, a, b);
VC4_FLOAT_PRINT_IMM(fdiv, "fdiv%s r%d, r%d, 0x%08x", cond, d, a, b);
VC4_FLOAT_PRINT_IMM(fcmp, "fcmp%s", cond);
VC4_FLOAT_PRINT_IMM(fabs, "fabs%s", cond);
VC4_FLOAT_PRINT_IMM(frsb, "frsb%s", cond);
VC4_FLOAT_PRINT_IMM(fmax, "fmax%s", cond);
VC4_FLOAT_PRINT_IMM(frcp, "frcp%s", cond);
VC4_FLOAT_PRINT_IMM(frsqrt, "frsqrt%s", cond);
VC4_FLOAT_PRINT_IMM(fnmul, "fnmul%s", cond);
VC4_FLOAT_PRINT_IMM(fmin, "fmin%s", cond);
VC4_FLOAT_PRINT_IMM(fld0, "fld0%s", cond);
VC4_FLOAT_PRINT_IMM(fld1, "fld1%s", cond);
VC4_FLOAT_PRINT_IMM(flog2, "flog2%s", cond);
VC4_FLOAT_PRINT_IMM(fexp2, "fexp2%s", cond);


struct vc4_float_op {
	void (*print)(int, int, int, const char *);
	void (*print_imm)(int, int, uint32_t, const char *);
};

#define FLOAT_OP_DECL(__op) {\
  .print = vc4_float_op_print_ ## __op,\
  .print_imm = vc4_float_op_print_imm_ ## __op\
}

static struct vc4_float_op vc4_float_op_table[] = {
	FLOAT_OP_DECL(fadd),
	FLOAT_OP_DECL(fsub),
	FLOAT_OP_DECL(fmul),
	FLOAT_OP_DECL(fdiv),
	FLOAT_OP_DECL(fcmp),
	FLOAT_OP_DECL(fabs),
	FLOAT_OP_DECL(frsb),
	FLOAT_OP_DECL(fmax),
	FLOAT_OP_DECL(frcp),
	FLOAT_OP_DECL(frsqrt),
	FLOAT_OP_DECL(fnmul),
	FLOAT_OP_DECL(fmin),
	FLOAT_OP_DECL(fld0),
	FLOAT_OP_DECL(fld1),
	FLOAT_OP_DECL(flog2),
	FLOAT_OP_DECL(fexp2),
};

#undef FLOAT_OP_DECL

#define vc4_float_op_print(index, p0, p1, p2, __cond) \
  vc4_float_op_table[index].print(p0, p1, p2, __cond)

#define vc4_float_op_print_imm(index, p0, p1, __imm, __cond) \
  vc4_float_op_table[index].print_imm(p0, p1, __imm, __cond)
