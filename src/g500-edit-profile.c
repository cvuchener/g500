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
#include <string.h>
#include <endian.h>

#include "g500.h"

const char *modifier_str[8] = { // Modifier in HID usage order (from 0x000500E0)
	"lctrl",
	"lshift",
	"lalt",
	"lmeta",
	"rctrl",
	"rshift",
	"ralt",
	"rmeta",
};

int main (int argc, char *argv[]) {
	int i, j;
	uint8_t buffer[G500_PAGE_SIZE];

	if (argc > 2 && strcmp (argv[1], "-h") == 0) {
		fprintf (stderr,
			"Usage: %s [command params...] [...]\n"
			"Commands are:\n"
			" - disable-mode <mode_nb>\n"
			" - dpi <mode_nb> <dpix> <dpiy>\n"
			" - leds <mode_nb> <led1> <led2> <led3> <led4>\n"
			" - angle on|off\n"
			" - default-mode <mode_nb>\n"
			" - refresh-rate <refresh_rate>\n"
			" - bind <button_num> <type> params...\n"
			"   Bind types are:\n"
			"    - mouse <button>\n"
			"    - keyboard [modifier+]usage\n"
			"    - cc usage\n"
			"    - special panleft|panright|dpi+|dpi-\n"
			"    - macro <page> <offset>\n"
			"    - disable\n",
			argv[0]);
		return EXIT_SUCCESS;
	}

	int ret, r = 0;
	do {
		ret = read (0, &buffer[r], G500_PAGE_SIZE-r);
		if (ret == -1) {
			perror ("read");
			return EXIT_FAILURE;
		}
		r += ret;
	} while (r != G500_PAGE_SIZE && ret != 0);

	struct g500_profile_t *profile = (struct g500_profile_t *)buffer;

	for (i = 1; i < argc;) {
		if (strcmp (argv[i], "disable-mode") == 0) {
			if (i+1 >= argc) {
				fprintf (stderr, "Not enough parameters, syntax is: disable-mode <mode_nb>.\n");
				return EXIT_FAILURE;
			}
			int mode = strtol (argv[i+1], NULL, 0);
			if (mode < 0 || mode >= 5) {
				fprintf (stderr, "Invalid mode number.\n");
				return EXIT_FAILURE;
			}
			memset (&profile->dpi_mode[mode], 0, sizeof (profile->dpi_mode[mode]));
			i += 2;
		}
		else if (strcmp (argv[i], "dpi") == 0) {
			if (i+3 >= argc) {
				fprintf (stderr, "Not enough parameters, syntax is: dpi <mode_nb> <dpix> <dpiy>.\n");
				return EXIT_FAILURE;
			}
			int mode = strtol (argv[i+1], NULL, 0);
			if (mode < 0 || mode >= 5) {
				fprintf (stderr, "Invalid mode number.\n");
				return EXIT_FAILURE;
			}
			for (j = 0; j < 2; ++j) {
				profile->dpi_mode[mode].resolution[j] = htobe16 (g500_pack_resolution (strtol (argv[i+2+j], NULL, 0)));
			}
			i += 4;
		}
		else if (strcmp (argv[i], "leds") == 0) {
			if (i+5 >= argc) {
				fprintf (stderr, "Not enough parameters, syntax is: leds <mode_nb> <led1> <led2> <led3> <led4>.\n");
				return EXIT_FAILURE;
			}
			int mode = strtol (argv[i+1], NULL, 0);
			if (mode < 0 || mode >= 5) {
				fprintf (stderr, "Invalid mode number.\n");
				return EXIT_FAILURE;
			}
			uint16_t leds = 0;
			for (j = 0; j < 4; ++j) {
				if (strcmp (argv[i+2+j], "on") == 0) {
					leds |= 2 << (4*j);
				}
				else if (strcmp (argv[i+2+j], "off") == 0) {
					leds |= 1 << (4*j);
				}
				else {
					fprintf (stderr, "Invalid value: \"%s\", must be \"on\" or \"off\".\n", argv[i+2+j]);
					return EXIT_FAILURE;
				}
			}
			profile->dpi_mode[mode].leds = htole16 (leds);
			i += 6;
		}
		else if (strcmp (argv[i], "angle") == 0) {
			if (i+1 >= argc) {
				fprintf (stderr, "Not enough parameters, syntax is: angle on|off.\n");
				return EXIT_FAILURE;
			}
			int angle;
			if (strcmp (argv[i+1], "on") == 0)
				angle = 1;
			else if (strcmp (argv[i+1], "off") == 0)
				angle = 0;
			else {
				fprintf (stderr, "Invalid value: \"%s\", must be \"on\" or \"off\".\n", argv[i+1]);
				return EXIT_FAILURE;
			}
			profile->angle = (angle ? 2 : 1);
			i += 2;
		}
		else if (strcmp (argv[i], "default-mode") == 0) {
			if (i+1 >= argc) {
				fprintf (stderr, "Not enough parameters, syntax is: default-mode <mode_nb>.\n");
				return EXIT_FAILURE;
			}
			profile->default_mode = strtol (argv[i+1], NULL, 0);
			i += 2;
		}
		else if (strcmp (argv[i], "refresh-rate") == 0) {
			if (i+1 >= argc) {
				fprintf (stderr, "Not enough parameters, syntax is: refresh-rate <refreshr_rate>.\n");
				return EXIT_FAILURE;
			}
			profile->refresh_rate = g500_pack_refresh_rate (strtol (argv[i+1], NULL, 0));
			i += 2;
		}
		else if (strcmp (argv[i], "bind") == 0) {
			if (i+2 >= argc) {
				fprintf (stderr, "Not enough parameters, syntax is: bind <button> mouse|keyboard|cc|special|macro|disable ...\n");
				return EXIT_FAILURE;
			}
			int button = strtol (argv[i+1], NULL, 0);
			if (button < 0 || button >= 13) {
				fprintf (stderr, "Invalid mouse button number: %d\n", button);
				return EXIT_FAILURE;
			}
			i += 2;
			if (strcmp (argv[i], "mouse") == 0) {
				if (i+1 >= argc) {
					fprintf (stderr, "Not enough parameters, syntax is: mouse <button>.\n");
					return EXIT_FAILURE;
				}
				int target_button = strtol (argv[i+1], NULL, 0);
				if (target_button < 0 || target_button >= 16) {
					fprintf (stderr, "Invalid target button number: %d\n", target_button);
					return EXIT_FAILURE;
				}
				memset (&profile->binding[button], 0, sizeof (profile->binding[button]));
				profile->binding[button].type = G500_BINDING_MOUSE_BUTTON;
				profile->binding[button].value.button = htole16 (1 << target_button);
				i += 2;
			}
			else if (strcmp (argv[i], "keyboard") == 0) {
				if (i+1 >= argc) {
					fprintf (stderr, "Not enough parameters, syntax is: keyboard [modifier+]...key.\n");
					return EXIT_FAILURE;
				}
				uint8_t modifier = 0;
				char *current = argv[i+1], *modifier_end;
				while ((modifier_end = strchr (current, '+'))) {
					int mod;
					for (j = 0; j < 8; ++j) {
						if (strncmp (current, modifier_str[j], modifier_end - current) == 0) {
							mod = j;
							break;
						}
					}
					if (j == 8) {
						fprintf (stderr, "Invalid modifier key: %*s, valid values are ", (int)(modifier_end - current), current);
						for (j = 0; j < 8; ++j) {
							if (j != 0)
								fprintf (stderr, " or ");
							fprintf (stderr, "%s", modifier_str[j]);
						}
						fprintf (stderr, ".\n");
						return EXIT_FAILURE;
					}
					modifier |= 1 << mod;
					current = modifier_end + 1;
				}
				int key = strtol (current, NULL, 0);
				if (key < 0 || key > 0xA4) {
					fprintf (stderr, "Keyboard usage must be between 0 and 0xA4.\n");
					return EXIT_FAILURE;
				}
				memset (&profile->binding[button], 0, sizeof (profile->binding[button]));
				profile->binding[button].type = G500_BINDING_KEY;
				profile->binding[button].value.key.modifier = modifier;
				profile->binding[button].value.key.usage = key;
				i += 2;
			}
			else if (strcmp (argv[i], "cc") == 0) {
				if (i+1 >= argc) {
					fprintf (stderr, "Not enough parameters, syntax is: cc <usage>.\n");
					return EXIT_FAILURE;
				}
				int usage = strtol (argv[i+1], NULL, 0);
				memset (&profile->binding[button], 0, sizeof (profile->binding[button]));
				profile->binding[button].type = G500_BINDING_CONSUMER_CONTROL;
				profile->binding[button].value.cc_usage = htobe16 (usage);
				i += 2;
			}
			else if (strcmp (argv[i], "special") == 0) {
				if (i+1 >= argc) {
					fprintf (stderr, "Not enough parameters, syntax is: special panleft|panright|dpi+|dpi-.\n");
					return EXIT_FAILURE;
				}
				uint16_t special;
				if (strcmp (argv[i+1], "panleft") == 0)
					special = G500_SPECIAL_BUTTON_PAN_LEFT;
				else if (strcmp (argv[i+1], "panright") == 0)
					special = G500_SPECIAL_BUTTON_PAN_RIGHT;
				else if (strcmp (argv[i+1], "dpi+") == 0)
					special = G500_SPECIAL_BUTTON_DPI_PLUS;
				else if (strcmp (argv[i+1], "dpi-") == 0)
					special = G500_SPECIAL_BUTTON_DPI_MINUS;
				else {
					fprintf (stderr, "Invalid special button %s\n", argv[i+1]);
					return EXIT_FAILURE;
				}
				memset (&profile->binding[button], 0, sizeof (profile->binding[button]));
				profile->binding[button].type = G500_BINDING_SPECIAL_BUTTON;
				profile->binding[button].value.button = htole16 (special);
				i += 2;
			}
			else if (strcmp (argv[i], "macro") == 0) {
				if (i+2 >= argc) {
					fprintf (stderr, "Not enough parameters, syntax is: macro <page> <offset>.\n");
					return EXIT_FAILURE;
				}
				uint8_t page = strtol (argv[i+1], NULL, 0);
				if (page < 0 || page > 0x7F) {
					fprintf (stderr, "Invalid macro page %d.\n", page);
					return EXIT_FAILURE;
				}
				uint8_t offset = strtol (argv[i+2], NULL, 0);
				profile->binding[button].type = page;
				profile->binding[button].value.macro.offset = offset;
				i += 3;
			}
			else if (strcmp (argv[i], "disable") == 0) {
				memset (&profile->binding[button], 0, sizeof (profile->binding[button]));
				profile->binding[button].type = G500_BINDING_UNSET;
				i += 1;
			}
			else {
				fprintf (stderr, "Invalid binding type: %s\n", argv[i+2]);
				return EXIT_FAILURE;
			}
		}
		else {
			fprintf (stderr, "Invalid command: %s.\n", argv[i]);
			return EXIT_FAILURE;
		}
	}
	write (1, profile, sizeof (struct g500_profile_t));
	return EXIT_SUCCESS;
}

