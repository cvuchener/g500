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

#include "logitech.h"
#include "g500.h"

int main (int argc, char *argv[]) {
	if (argc < 3 || argc > 5) {
		fprintf (stderr, "Usage: %s hidraw len [page] [offset]\n", argv[0]);
		return EXIT_FAILURE;
	}
	int fd = open (argv[1], O_RDWR);
	if (fd == -1) {
		perror ("open");
		return EXIT_FAILURE;
	}
	uint16_t len = strtol (argv[2], NULL, 0);
	uint8_t page = 0, offset = 0;
	if (argc > 3)
		page = strtol (argv[3], NULL, 0);
	if (argc > 4)
		offset = strtol (argv[4], NULL, 0);

	uint8_t *buffer = malloc (len);
	if (!buffer) {
		fprintf (stderr, "Error: not enough memory.\n");
		return EXIT_FAILURE;
	}
	int r = 0, ret;
	while (0 != (ret = read (0, &buffer[r], len - r))) {
		if (ret == -1) {
			perror ("read");
			return EXIT_FAILURE;
		}
		r += ret;
	}
	if (r < len) {
		fprintf (stderr, "Warning: not enough data read, sending %d bytes.\n", r);
		len = r;
	}

	if (-1 == g500_write_page (fd, page, offset, buffer, len)) {
		return EXIT_FAILURE;
	}

	free (buffer);

	return EXIT_SUCCESS;
}
