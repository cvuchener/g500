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
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <endian.h>

#include "g500.h"

const char *usage = "Usage: %s hidraw default|disable|<pagenum>\n";

int main (int argc, char *argv[]) {
	if (argc != 3) {
		fprintf (stderr, usage, argv[0]);
		return EXIT_FAILURE;
	}

	int fd = open (argv[1], O_RDWR);
	if (-1 == fd) {
		perror ("open");
		return EXIT_FAILURE;
	}

	if (strcmp (argv[2], "default") == 0) {
		if (-1 == g500_use_default_profile (fd)) {
			fprintf (stderr, "Failed to set profile.\n");
			return EXIT_FAILURE;
		}
	}
	else if (strcmp (argv[2], "disable") == 0) {
		if (-1 == g500_disable_profile (fd)) {
			fprintf (stderr, "Failed to set profile.\n");
			return EXIT_FAILURE;
		}
	}
	else {
		char *endptr;
		int page = strtol (argv[2], &endptr, 0);
		if (endptr == argv[2] || *endptr != '\0') {
			fprintf (stderr, usage, argv[0]);
			return EXIT_FAILURE;
		}
		if (-1 == g500_use_profile (fd, page)) {
			fprintf (stderr, "Failed to set profile.\n");
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

