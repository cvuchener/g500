/*
 * Copyright 2015 Clément Vuchener
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

#include "logitech.h"
#include "g500.h"

enum option_val {
	OPT_OPERATION,
	OPT_PAGE,
	OPT_OFFSET,
	OPT_LENGTH,
	OPT_SRC_OFFSET,
	OPT_SRC_LENGTH,
};
const static struct option options[] = {
	{ "op", required_argument, NULL, OPT_OPERATION },
	{ "page", required_argument, NULL, OPT_PAGE },
	{ "offset", required_argument, NULL, OPT_OFFSET },
	{ "length", required_argument, NULL, OPT_LENGTH },
	{ "src-offset", required_argument, NULL, OPT_SRC_OFFSET },
	{ "src-length", required_argument, NULL, OPT_SRC_LENGTH },
	{ 0, 0, 0, 0 }
};
const static char *usage = "Usage: %s [options] hidraw\n"
	"Options are:\n"
	" --op operation\n"
	" --page page\n"
	" --offset offset\n"
	" --length length\n"
	" --src-offset offset\n"
	" --src-length length\n"
;

int main (int argc, char *argv[]) {
	struct g500_mem_op_params_t params;
	memset (&params, 0, sizeof (struct g500_mem_op_params_t));
	int val;
	while (-1 != (val = getopt_long (argc, argv, "", options, NULL))) {
		switch (val) {
		case OPT_OPERATION:
			params.op = strtol (optarg, NULL, 0);
			break;
		case OPT_PAGE:
			params.page = strtol (optarg, NULL, 0);
			break;
		case OPT_OFFSET:
			params.offset = strtol (optarg, NULL, 0);
			break;
		case OPT_LENGTH:
			params.len = htobe16 (strtol (optarg, NULL, 0));
			break;
		case OPT_SRC_OFFSET:
			params.src_offset = strtol (optarg, NULL, 0);
			break;
		case OPT_SRC_LENGTH:
			params.src_len = strtol (optarg, NULL, 0);
			break;
		default:
			fprintf (stderr, "Invalid option.\n");
			fprintf (stderr, usage, argv[0]);
			return EXIT_FAILURE;
		}
	}

	if (optind != argc-1) {
		fprintf (stderr, "Too much argument.\n");
		fprintf (stderr, usage, argv[0]);
		return EXIT_FAILURE;
	}

	int fd = open (argv[optind], O_RDWR);
	if (-1 == fd) {
		perror ("open");
		return EXIT_FAILURE;
	}


	if (-1 == logitech_query (fd, LOGITECH_SEND_LONG, G500_QUERY_TYPE_MEMORY_OP, (uint8_t *)&params, NULL)) {
		fprintf (stderr, "Error during operation\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

