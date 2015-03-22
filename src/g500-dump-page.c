#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "logitech.h"
#include "g500.h"

int main (int argc, char *argv[]) {
	if (argc < 3) {
		fprintf (stderr, "Usage: %s /dev/hidrawN page\n", argv[0]);
		return EXIT_FAILURE;
	}

	int fd = open (argv[1], O_RDWR);
	if (-1 == fd) {
		perror ("open");
		return EXIT_FAILURE;
	}

	uint8_t page = strtol (argv[2], NULL, 0);

	uint8_t buffer[G500_PAGE_SIZE];
	if (-1 == g500_read_mem (fd, page, 0, buffer, G500_PAGE_SIZE)) {
		fprintf (stderr, "Error while reading page\n");
		return EXIT_FAILURE;
	}

	if (-1 == write (1, buffer, G500_PAGE_SIZE)) {
		perror ("write");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
