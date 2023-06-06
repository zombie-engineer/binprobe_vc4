#pragma once
#include <stdint.h>

struct vc4_disas;
typedef void (*vc4_print_instruction_fn)(struct vc4_disas *d, uint16_t *bytecode);

struct vc4_disas_instruction {
	vc4_print_instruction_fn print;
};

int vc4_decode_instr(uint16_t *i, int i_sz, struct vc4_disas_instruction *res);
