#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char *argv[]) {
	int ret, fd;
	char buffer[64];

	if (argc != 2) {
		fprintf (stderr, "Usage: %s file\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (-1 == (fd = open (argv[1], O_RDWR))) {
		perror ("open");
		return EXIT_FAILURE;
	}

	if (-1 == (ret = read (0, buffer, sizeof (buffer)))) {
		perror ("read");
		return EXIT_FAILURE;
	}

	if (-1 == write (fd, buffer, ret)) {
		perror ("write");
		return EXIT_FAILURE;
	}

	if (-1 == (ret = read (fd, buffer, sizeof (buffer)))) {
		perror ("read");
		return EXIT_FAILURE;
	}

	if (-1 == write (1, buffer, ret)) {
		perror ("write");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

