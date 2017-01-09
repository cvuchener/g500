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

#include "g700s.h"

const char *onoff_to_string (int value);

int main (int argc, char *argv[]) {
	int i, j;
	uint8_t buffer[G700S_PAGE_SIZE];

	int ret, r = 0;
	do {
		ret = read (0, &buffer[r], G700S_PAGE_SIZE-r);
		if (ret == -1) {
			perror ("read");
			return EXIT_FAILURE;
		}
		r += ret;
	} while (r != G700S_PAGE_SIZE && ret != 0);

	struct g700s_profile_t *profile = (struct g700s_profile_t *)buffer;

	printf ("dpi modes:\n");
	for (i = 0; i < 5; ++i) {
		printf (" - Mode %d: %ddpi %ddpi",
			i,
			g700s_unpack_resolution (profile->dpi_mode[i].resolution[0]),
			g700s_unpack_resolution (profile->dpi_mode[i].resolution[1])
			);
		for (j = 0; j < 4; ++j) {
			int led = (le16toh (profile->dpi_mode[i].leds) & (0x0F << 4*j)) >> 4*j;
			printf (" %s", onoff_to_string (led));
		}
		printf ("\n");
	}

	printf ("angle correction: %s\n", onoff_to_string (profile->angle));
	printf ("default dpi mode: %d\n", profile->default_mode);
	printf ("refresh rate: %dHz\n", g700s_unpack_refresh_rate (profile->refresh_rate));
	printf ("Unknown bytes:\n25: %x\n26: %x\n27: %x\n28: %x\n29: %x\n30: %x\n",
			profile->unk[0],
			profile->unk[1],
			profile->unk[2],
			profile->unk[3],
			profile->unk[4],
			profile->unk[5]);


	printf ("bindings:\n");
	for (i = 0; i < 13; ++i) {
		//printf (" - button %d -> ", i);
        printf ("- %-16s -> ",g700s_button_name(i));

		if (profile->binding[i].type <= G700S_BINDING_MACRO_MAX) {
			printf ("macro (0x%02hhX:0x%02hhX)",
				profile->binding[i].type,
				profile->binding[i].value.macro.offset);
		}
		else switch (profile->binding[i].type) {
		case G700S_BINDING_MOUSE_BUTTON:
			printf ("mouse button %d",
				g700s_get_button_num (le16toh (profile->binding[i].value.button)));
			break;
		case G700S_BINDING_KEY:
			printf ("key ");
			for (j = 0; j < 8; ++j) {
				if (profile->binding[i].value.key.modifier & (1<<j))
				    //	printf ("0x%X + ", 0xE0+j); // 0xE0 is the minimum usage for modifier keys
                    switch(0xE0 + j){
                        case 0xE0:
                            printf("LeftControl ");
                            break;
               	        case 0xE1:
                            printf("LeftShift ");
                            break;
                        case 0xE2:
                            printf("LeftAlt ");
                            break;
                        case 0xE3:
                            printf("Left GUI ");
                            break;
                        case 0xE4:
                            printf("RightControl ");
                            break;
                        case 0xE5:
                            printf("RightShift ");
                            break;
                        case 0xE6:
                            printf("RightAlt ");
                            break;
                        case 0xE7:
                            printf("Right GUI ");
                            break;
                    }
			}
			if ((profile->binding[i].value.key.usage > 0x03) && (profile->binding[i].value.key.usage < 0x1E))
				printf ("%c", profile->binding[i].value.key.usage - 0x04 + 0x41);
			else
				printf ("0x%02hhX", profile->binding[i].value.key.usage);
			break;
		case G700S_BINDING_SPECIAL_BUTTON:
			printf ("special ");
			switch (le16toh(profile->binding[i].value.button)) {
			case G700S_SPECIAL_BUTTON_PAN_LEFT:
				printf ("pan left");
				break;
			case G700S_SPECIAL_BUTTON_PAN_RIGHT:
				printf ("pan right");
				break;
			case G700S_SPECIAL_BUTTON_DPI_PLUS:
				printf ("dpi +");
				break;
			case G700S_SPECIAL_BUTTON_DPI_MINUS:
				printf ("dpi -");
				break;
			case G700S_SPECIAL_BUTTON_BATTERY:
				printf ("battery level");
				break;
			case G700S_SPECIAL_BUTTON_PROFILE_CYC:
				printf ("cycle profile");
				break;
			case G700S_SPECIAL_BUTTON_DPI_CYC_DOWN:
				printf ("cycle dpi down");
				break;
			case G700S_SPECIAL_BUTTON_DPI_CYC_UP:
				printf ("cycle dpi up");
				break;
			default:
				printf ("unknown %x", le16toh(profile->binding[i].value.button));
			}
			break;
		case G700S_BINDING_CONSUMER_CONTROL:
			printf ("consumer control 0x%X", be16toh (profile->binding[i].value.cc_usage));
			break;
		case G700S_BINDING_UNSET:
			printf ("unset");
			break;
		default:
			printf ("unknown (0x%02hhX) %02hhX %02hhX",
				profile->binding[i].type,
				profile->binding[i].value.bytes[0],
				profile->binding[i].value.bytes[1]);
		}
		printf ("\n");
	}
	return EXIT_SUCCESS;
}

const char *onoff_to_string (int value) {
	static char error_str[32];
	switch (value) {
	case 1:
		return "off";
	case 2:
		return "on";
	default:
		snprintf (error_str, sizeof (error_str), "invalid (%d)", value);
		return error_str;
	}
}
