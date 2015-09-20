#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "logitech.h"

const char *usage = "Usage: %s hidraw command read|write short|long parameters...\n";

int main (int argc, char *argv[])
{
	int ret, i;
	char *endptr;
	int fd;
	int command;
	uint8_t type;
	uint8_t params[16] = {0}, data[16];
	size_t params_size, data_size;

	if (argc < 5) {
		fprintf (stderr, usage, argv[0]);
		return EXIT_FAILURE;
	}

	fd = open (argv[1], O_RDWR);
	if (fd == -1) {
		perror ("open");
		return EXIT_FAILURE;
	}

	command = strtol (argv[2], &endptr, 0);
	if (*endptr != '\0' || command < 0 || command > 255) {
		fprintf (stderr, "Invalid command: %s.\n", argv[2]);
		return EXIT_FAILURE;
	}

	type = 0x80;
	if (strcmp (argv[3], "read") == 0)
		type |= 0x01;
	else if (strcmp (argv[3], "write") != 0) {
		fprintf (stderr, "Must be read or write.\n");
		return EXIT_FAILURE;
	}
	if (strcmp (argv[4], "long") == 0)
		type |= 0x02;
	else if (strcmp (argv[4], "short") != 0) {
		fprintf (stderr, "Must be short or long.\n");
		return EXIT_FAILURE;
	}

	params_size = (type == LOGITECH_SEND_LONG ? 16 : 3);
	for (i = 5; i < argc; ++i) {
		if (i-5 >= params_size) {
			fprintf (stderr, "Too many parameters.\n");
			return EXIT_FAILURE;
		}
		int p = strtol (argv[i], &endptr, 16);
		if (*endptr != '\0' || p < 0 || p > 255) {
			fprintf (stderr, "Invalid parameter: %s.\n", argv[i]);
			return EXIT_FAILURE;
		}
		params[i-5] = p;
	}

	ret = logitech_query (fd, type, command, params, data);
	if (ret == -1) {
		fprintf (stderr, "Command failed.\n");
		return EXIT_FAILURE;
	}
	if (ret > 0) {
		if (ret <= LOGITECH_ERROR_MAX)
			fprintf (stderr, "Error: %s.\n", logitech_error_string[ret]);
		else
			fprintf (stderr, "Unknown error: %d.\n", ret);
		return ret;
	}

	data_size = (type == LOGITECH_READ_LONG ? 16 : 3);
	for (i = 0; i < data_size; ++i) {
		if (i != 0)
			printf (" ");
		printf ("%02x", data[i]);
	}
	printf ("\n");

	return EXIT_SUCCESS;
}

