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

#include "logitech.h"
#include "g500.h"

int main (int argc, char *argv[]) {
	if (argc < 5) {
		fprintf (stderr, "Usage: %s /dev/hidrawN page offset len\n", argv[0]);
		return EXIT_FAILURE;
	}

	int fd = open (argv[1], O_RDWR);
	if (-1 == fd) {
		perror ("open");
		return EXIT_FAILURE;
	}

	uint8_t page = strtol (argv[2], NULL, 0);
	uint8_t offset = strtol (argv[3], NULL, 0);
	int len = strtol (argv[4], NULL, 0);

	uint8_t *buffer = malloc (len);
	if (-1 == g500_read_mem (fd, page, offset, buffer, len)) {
		fprintf (stderr, "Error while reading page\n");
		return EXIT_FAILURE;
	}

	if (-1 == write (1, buffer, len)) {
		perror ("write");
		return EXIT_FAILURE;
	}

	free (buffer);

	return EXIT_SUCCESS;
}
