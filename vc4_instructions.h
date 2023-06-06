#pragma once
#include <stdint.h>
#include <string.h>
#include "bitfield.h"

struct vc4_disas;

int vc4_cond_from_str(const char *buf);

void print_abs_addr(struct vc4_disas *d, uint32_t addr, int as_primary);

void vc4_i_print_i16_illegal(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_add_sp(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_stub(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_bkpt(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_nop(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_sleep(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_user(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_ei(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_di(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_cbclr(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_cbadd1(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_cbadd2(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_cbadd3(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_rti(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_swi(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_b(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_switch(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_version(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_swi_u(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_ldm_stm(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_ld_st_sp(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_ldw_stw(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_b_cond(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_ld_st(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i16_alu_op(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i32_p_control(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i32_stub(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i32_addcmpb(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i32_b(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i32_ld_st_cond_indexed(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i32_ld_st_disp_12(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i32_ld_st_pre_post(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i32_ld_st_disp_16(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i32_alu_op(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i32_alu_add(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i32_alu_add_pc(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i32_alu_cond(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i32_alu_fop(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i32_ftrunc(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i32_floor(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i32_flts(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i32_fltu(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i48_b(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i48_add_pc_rel(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i48_add_ld_st(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i48_alu_op(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i48_alu_add(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i48_stub(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i48_mem_op(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_i48_data_vop(struct vc4_disas *dis, uint16_t *instr);
void vc4_i_print_i80_mem_op(struct vc4_disas *dis, uint16_t *instr);
void vc4_i_print_i80_data_op(struct vc4_disas *dis, uint16_t *instr);
void vc4_i_print_i80_stub(struct vc4_disas *d, uint16_t *instr);

void vc4_i_print_instruction_52(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_instruction_53(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_instruction_54(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_instruction_55(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_instruction_56(struct vc4_disas *d, uint16_t *instr);
void vc4_i_print_instruction_58(struct vc4_disas *d, uint16_t *instr);
