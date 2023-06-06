#include "symbol_table.h"
#include "vc4_log.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <limits.h>
#include <unistd.h>
#include "filereader.h"

const struct symbol *symbol_table_find_by_name(struct symbol_table *st,
  const char *symbol_name)
{
	const struct symbol *s, *send;

	s = st->data;
	send = s + st->num_elements;

	while(s < send) {
		if (!strcmp(s->name, symbol_name))
			return s;
		s++;
	}

	return NULL;
}

const struct symbol *symbol_table_find_symbol(struct symbol_table *st,
  size_t addr)
{
	const struct symbol *s, *send;

	s = st->data;
	send = s + st->num_elements;
#if 0
	uint32_t offset = 0;
	if ((addr >> 24) != 0x7e)
		offset = 0x80000000;
#endif


	while(s < send) {
		if (s->addr == addr)
			return s;
		s++;
	}

	return NULL;
}

void symbol_table_printout(const struct symbol_table *st)
{
	int i;
	char typechars[] = { 'd', 's', 'f', 'p', 'S', 'w' };
	const struct symbol *s;

	printf("Symbol table:\n");
	for (i = 0; i < st->num_elements; ++i) {
		s = &st->data[i];
		printf("  0x%08lx:%c:%s:%d\n", s->addr, typechars[s->type], s->name, s->size);
	}
	printf("\n");
}

int symbol_table_add(struct symbol_table *st, struct symbol *sym)
{
	struct symbol *tmp_data = st->data;
	struct symbol *s;

	if (st->num_elements == st->max_elements) {
		st->max_elements *= 1.6;
		st->data = (struct symbol *)malloc(sizeof(struct symbol) * st->max_elements);
		if (!st->data)
			return -1;

		memcpy(st->data, tmp_data, st->num_elements * sizeof(struct symbol));
		free(tmp_data);
	}
	s = &st->data[st->num_elements++];
	memcpy(s, sym, sizeof(*s));
	printf("s: %s %p:%s\n", s->name, st->data, st->data[2].name);
	return 0;
}

int symbol_table_add_simple(struct symbol_table *st, const char *name, uint32_t addr, int type)
{
	struct symbol s;

	memset(&s, 0, sizeof(s));
	strcpy(s.name, name);
	s.addr = addr;
	s.type = type;
	return symbol_table_add(st, &s);
}

static int symbol_table_parse_line(const char *line, struct symbol *s)
{
	char *p;
	char *p1;
	unsigned long addr;
	size_t len;

	errno = 0;
	addr = strtoul(line, &p, 16);

	if (addr == ULONG_MAX && errno) {
		log_errno("Failed to parse address in line %s", line);
		return -1;
	}
	if (*p != ':') {
		log_err("Invalid line format, expected 'address:sybmol_name'");
		return -1;
	}
	p++;
	s->type = SYMBOL_TYPE_FUNCTION;
	s->size_known = false;
	s->addr = addr;
	s->num_array_elements = 1;
	/* data type delimeter */
	p1 = strchr(p, ':');
	if (p1) {
		switch(*p++) {
		case 'd':
			s->type = SYMBOL_TYPE_DATA;
			break;
		case 'w':
			s->type = SYMBOL_TYPE_WORD;
			break;
		case 'f':
			s->type = SYMBOL_TYPE_FUNCTION;
			break;
		case 's':
			s->type = SYMBOL_TYPE_STRING;
			break;
		case 'S':
			s->type = SYMBOL_TYPE_STRUCT_SPEC;
			if (*p != '{')
				abort();

			p1 = p + 1;
			do {
				p++;
			}
			while(*p != '}');
			p++;

			if ((unsigned long)(p - p1 - 1) > sizeof(s->struct_spec) - 1)
				abort();

			memcpy(s->struct_spec, p1, p - p1 - 1);
			break;
		case 'p':
			s->type = SYMBOL_TYPE_PTR;
			break;
		default:
			s->type = SYMBOL_TYPE_FUNCTION;
			break;
		}
		if (*p == '@') {
			p++;
			s->size = strtoul(p, &p, 10);
			s->size_known = true;
		}

		if (*p == '[') {
			p++;
			p1 = p;
			while(*p != ']')
				p++;
			p++;
			s->num_array_elements = atoi(p1);
		}

		if (*p != ':')
			abort();
		p++;
	}

	len = strlen(p);
	if (len > sizeof(s->name) - 1) {
		log_err("symbol name exceeds allowed %d chars", sizeof(s->name) - 1);
		return -1;
	}
	memcpy(s->name, p, len);
	s->name[len] = 0;

	return 0;
}

struct filereader_iter_symbol_lines_ctx {
	int err;
	struct symbol_table *st;
};

int filereader_iter_symbol_lines(const char *line, void *arg)
{
	int ret;
	struct filereader_iter_symbol_lines_ctx *ctx = arg;
	struct symbol s;
	memset(&s, 0, sizeof(s));

	if (symbol_table_parse_line(line, &s)) {
		log_err("Failed to parse line %s", line);
		goto err;
	}

	ret = symbol_table_add(ctx->st, &s);
	if (ret) {
		log_err("Failed to add symbol %s to symbol_table: %d", s.name, ret);
		goto err;
	}

	return FILEREADER_ITER_CONTINUE;

err:
	ctx->err = -1;
	return FILEREADER_ITER_STOP;
}

int symbol_table_init(struct symbol_table *st, const char *symbols_filepath)
{
	struct filereader f;
	int ret;
	struct filereader_iter_symbol_lines_ctx ctx;

	st->data = malloc(sizeof(struct symbol) * SYMBOL_TABLE_INIT_SIZE);
	if (!st->data)
		return -1;

	st->num_elements = 0;
	st->max_elements = SYMBOL_TABLE_INIT_SIZE;

	ctx.err = 0;
	ctx.st = st;

	if (symbols_filepath) {
		ret = filereader_open(&f, symbols_filepath);
		if (ret) {
			printf("symbol_table_init failed: filereader_open failed: %d\n", ret);
			return ret;
		}
	}

	ret = filereader_iterate_lines(&f, filereader_iter_symbol_lines, &ctx);
	if (ret) {
		return ret;
	}

	return 0;
}

