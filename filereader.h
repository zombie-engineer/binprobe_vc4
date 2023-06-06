#pragma once

struct filereader {
	int fd;
	char buf[16];
	int zero;
	int off;
	int off2;
	int eof;
};

typedef int (*filereader_cb_t)(const char *line, void *ctx);
/*
 * Returns 0 on success, -1 on notsuccess
 */
int filereader_open(struct filereader *f, const char *path);

#define FILEREADER_ITER_CONTINUE 0
#define FILEREADER_ITER_STOP     1
int filereader_iterate_lines(struct filereader *fr, filereader_cb_t cb, void *ctx);


