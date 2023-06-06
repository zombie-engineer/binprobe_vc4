#pragma once
#include "symbol_table.h"

struct vc4_disas {
  uint32_t current_pc;
  uint32_t base_addr;
  uint32_t data_start;
  struct symbol_table symbols;
};
