#include "filereader.h"
#include <errno.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define min(a, b) ((a < b) ? a : b)

static int filereader_read(struct filereader *f, char *data, int length)
{
	int ret;
	int left = length;
	int in_cache;
	int to_copy;
	int copied = 0;

	while (left) {
		in_cache = f->off2 - f->off;
		if (!in_cache) {
			/* cache refill */

			if (f->eof)
				return copied;

			ret = read(f->fd, f->buf, sizeof(f->buf));
			if (ret < 0) {
				perror("Failed to read source from file\n");
				return -1;
			}
			f->off2 = ret;

			/* zero-terminate a bit complex */
			if (ret < (int)sizeof(f->buf))
				f->buf[ret] = 0;
			else
				f->zero = 0;

			if (ret != sizeof(f->buf))
				f->eof = 1;

			f->off = 0;
			in_cache = f->off2 - f->off;
		}

		to_copy = min(in_cache, left);
		memcpy(data + copied, f->buf + f->off, to_copy);
		f->off += to_copy;
		left -= to_copy;
		copied += to_copy;
	}
	return copied;
}

struct linefiller {
	char line[128];
	char *line_w;
};

static void linefiller_init(struct linefiller *l)
{
	l->line_w = l->line;
}

static int linefiller_fill(struct linefiller *l, char **chunk, int numbytes, filereader_cb_t cb, void *ctx)
{
	int ret;
	int i;
	char *chunk_p = *chunk;

	for (i = 0; i < numbytes; ++i) {
		char c = *chunk_p++;
		if (c != '\n') {
			*l->line_w++ = c;
		} else {
			*l->line_w = 0;
			ret = cb(l->line, ctx);
			if (ret == FILEREADER_ITER_STOP)
				break;
			l->line_w = l->line;
			memset(l->line, 0, sizeof(l->line));
		}
	}

	*chunk = chunk_p;
	return ret;
}

int filereader_iterate_lines(struct filereader *f, filereader_cb_t cb, void *ctx)
{
	int ret;
	char chunk[8];
	struct linefiller l;
	char *chunk_e = chunk + sizeof(chunk);
	char *chunk_p;

	if (!f) {
		printf("Filereader argument not provided\n");
		ret = -1;
		goto out_err;
	}

	linefiller_init(&l);

	while(1) {
		chunk_p = chunk;
		ret = filereader_read(f, chunk, sizeof(chunk));
		if (ret < 0)
			goto out_err;

		if (ret < (int)sizeof(chunk)) {
			if (f->eof) {
				if (ret < 0) {
					printf("Logic error filereader ret < 0\n");
					abort();
				}

				chunk_e = chunk + ret;
				ret = linefiller_fill(&l, &chunk_p, chunk_e - chunk_p, cb, ctx);
//				linefiller_flush(&l, cb, ctx);
				goto out_success;
			}

			if (ret >= 0) {
				printf("Unexpected error while iterating\n");
				ret = -1;
			}

			goto out_err;
		}

		while(chunk_p != chunk_e) {
			ret = linefiller_fill(&l, &chunk_p, chunk_e - chunk_p, cb, ctx);
			if (ret == FILEREADER_ITER_STOP)
				goto out_success;
		}
	}

out_success:
	return 0;

out_err:
	return ret;
}

int filereader_open(struct filereader *f, const char *path)
{
	f->fd = open(path, O_RDONLY);
	if (f->fd == -1) {
		printf("Failed to read source file %s. Error: %d (%s).\n", path, errno, strerror(errno));
		return -1;
	}
	f->eof = 0;
	f->zero = 0;
	f->off = sizeof(f->buf);
	f->off2 = f->off;
	memset(f->buf, 0, sizeof(f->buf));
	return 0;
}
