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
