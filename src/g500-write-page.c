#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "logitech.h"
#include "g500.h"

int main (int argc, char *argv[]) {
	if (argc < 2 || argc > 3) {
		fprintf (stderr, "Usage: %s hidraw [page]\n", argv[0]);
		return EXIT_FAILURE;
	}
	int fd = open (argv[1], O_RDWR);
	if (fd == -1) {
		perror ("open");
		return EXIT_FAILURE;
	}
	uint8_t page = 0;
	if (argc > 2)
		page = strtol (argv[2], NULL, 0);

	uint8_t buffer[512];
	int len = 0, ret;
	while (0 != (ret = read (0, &buffer[len], sizeof (buffer) - len))) {
		if (ret == -1) {
			perror ("read");
			return EXIT_FAILURE;
		}
		len += ret;
	}

	struct g500_mem_op_params_t params;
	memset (&params, 0, sizeof (struct g500_mem_op_params_t));
	params.op = 2;
	params.page = page;
	if (-1 == logitech_query (fd, LOGITECH_SEND_LONG, G500_QUERY_TYPE_MEMORY_OP, (uint8_t *)&params, NULL)) {
		fprintf (stderr, "Error during operation\n");
		return EXIT_FAILURE;
	}

	if (-1 == g500_write_page (fd, page, 0, buffer, 256)) {
		return EXIT_FAILURE;
	}
	if (-1 == g500_write_page (fd, page, 0x80, buffer+256, 256)) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
