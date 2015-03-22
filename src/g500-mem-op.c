#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "logitech.h"
#include "g500.h"

int main (int argc, char *argv[]) {
	struct g500_mem_op_params_t params;
	memset (&params, 0, sizeof (struct g500_mem_op_params_t));

	int fd = open (argv[1], O_RDWR);
	if (-1 == fd) {
		perror ("open");
		return EXIT_FAILURE;
	}

	if (argc > 2)
		params.op = strtol (argv[2], NULL, 0);
	if (argc > 3)
		params.page = strtol (argv[3], NULL, 0);
	if (argc > 4)
		params.offset = strtol (argv[4], NULL, 0);
	if (argc > 5)
		params.len = htobe16 (strtol (argv[5], NULL, 0));
	

	if (-1 == logitech_query (fd, LOGITECH_SEND_LONG, G500_QUERY_TYPE_MEMORY_OP, (uint8_t *)&params, NULL)) {
		fprintf (stderr, "Error during operation\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

