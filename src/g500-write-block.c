#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "logitech.h"
#include "g500.h"

int main (int argc, char *argv[]) {
	if (argc < 2 || argc > 4) {
		fprintf (stderr, "Usage: %s hidraw [page] [offset]\n", argv[0]);
		return EXIT_FAILURE;
	}
	int fd = open (argv[1], O_RDWR);
	if (fd == -1) {
		perror ("open");
		return EXIT_FAILURE;
	}
	uint8_t page = 0, offset = 0;
	if (argc > 2)
		page = strtol (argv[2], NULL, 0);
	if (argc > 3)
		offset = strtol (argv[3], NULL, 0);

	uint8_t buffer[256];
	int len = 0, ret;
	while (0 != (ret = read (0, &buffer[len], sizeof (buffer) - len))) {
		if (ret == -1) {
			perror ("read");
			return EXIT_FAILURE;
		}
		len += ret;
	}

	if (-1 == g500_write_page (fd, page, offset, buffer, len)) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
