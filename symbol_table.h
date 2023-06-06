#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define SYMBOL_TYPE_DATA 0
#define SYMBOL_TYPE_STRING 1
#define SYMBOL_TYPE_FUNCTION 2
#define SYMBOL_TYPE_PTR 3
#define SYMBOL_TYPE_STRUCT_SPEC 4
#define SYMBOL_TYPE_WORD 5

#define MAX_SYMBOL_NAME_SIZE 64
#define SYMBOL_TABLE_INIT_SIZE 8

struct symbol {
	char name[MAX_SYMBOL_NAME_SIZE];
	char term_char;
	uint64_t addr;
	int type;
	char struct_spec[32];
	int num_array_elements;
	int size;
	bool size_known;
};

struct symbol_table {
	struct symbol *data;
#define MAX_SYMBOL_TABLE_SIZE (1024 * 16)
	ssize_t num_elements;
	ssize_t max_elements;
};

int symbol_table_init(struct symbol_table *st, const char *init_symfile);
// int symbol_table_from_fd(struct symbol_table *st, int fd);
// int symbol_table_from_filepath(struct symbol_table *st, const char *symbols_filepath);
const struct symbol *symbol_table_find_symbol(struct symbol_table *st, size_t addr);
const struct symbol *symbol_table_find_by_name(struct symbol_table *st, const char *symbol_name); 
void symbol_table_printout(const struct symbol_table *st);
int symbol_table_add_simple(struct symbol_table *st, const char *name, uint32_t addr, int type);
