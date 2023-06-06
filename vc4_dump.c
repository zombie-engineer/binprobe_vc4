#include "vc4_decode.h"
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

static int fetch_next_instr(int fd, uint16_t i[5], int *i_sz)
{
	int size = 2;

	if (read(fd, i, 2) != 2)
		goto err;

	if (!(i[0] & 0x8000))
		goto out;

	size += 2;

	if (read(fd, i + 2, 2) != 2)
		goto err;

	if (i[0] <= 0xe000)
		goto out;

	size += 2;

	if (read(fd, i + 2, 2) != 2)
		goto err;

	if ((i[0] & 0xfc00) <= 0xf400)
		goto out;

	size += 4;
	if (read(fd, i + 3, 4) != 4)
		goto err;

out:
	*i_sz = size;
	return 0;

err:
	return -1;
}

static void dump_bytecode(uint32_t addr, uint16_t i[5], int i_sz)
{
	int n;

	printf("0x%08x: ", addr);
	for (n = 0; n < i_sz / 2; ++n) {

		printf("%02x %02x ",
			(i[n] >> 8) & 0xff,
			(i[n] >> 0) & 0xff);
	}

	for (n = n; n < 5; n++)
		printf("      ");

	printf(" | ");
}

int vc4_dump(int fd)
{
	int ret;
	uint16_t instr[5];
	int instr_sz;
	uint32_t addr = 0;

	ret = lseek(fd, 512, SEEK_SET);
	if (ret < 0) {
		perror("lseek");
		return -1;
	}

	while (1) {
		int i;
		struct vc4_disas_instruction dinstr;

		ret = fetch_next_instr(fd, instr, &instr_sz);
		if (ret) {
			printf("Failed to fetch next instruction: %d\n", ret);
			return ret;
		}

		ret = vc4_decode_instr(instr, instr_sz, &dinstr);
		if (ret) {
			const uint8_t *bytecode = (const uint8_t *)instr;
			printf("Failed to decode instruction: ");
			for (i = 0; i < instr_sz; ++i) {
				printf(" %02x", bytecode[i]);
			}
		}
		dump_bytecode(addr, instr, instr_sz);


		addr += instr_sz;
		printf("\n");
	}
	return 0;
}

int main(int argc, const char **argv)
{
	int ret;
	int fd;
	long bin_size;
	struct stat st;

	const char *binfile = NULL;
	binfile = argv[1];

	if (argc < 2) {
		printf("No parameters\n");
		return -1;
	}

	if (stat(binfile, &st) == -1) {
		perror("lseek failed");
		return -1;
	}

	fd = open(binfile, O_RDONLY);
	if (fd == -1) {
		perror("Failed to open binary file");
		return -1;
	}

	printf("Binfile opened: '%s', size:%ld\n", binfile, st.st_size);

	ret = vc4_dump(fd);
	printf("vc_dump finished with status %d\n", ret);
	return ret;
}
