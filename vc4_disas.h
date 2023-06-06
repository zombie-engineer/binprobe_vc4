#pragma once
#include "symbol_table.h"

struct vc4_disas {
  uint32_t *instr_buffer;
  uint32_t base_addr;
  uint32_t data_start;
  struct symbol_table symbols;
};
