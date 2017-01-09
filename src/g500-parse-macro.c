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

int get_button_num (uint16_t bits);

int main (int argc, char *argv[]) {
	uint8_t buffer[G500_PAGE_SIZE];

	int ret, r = 0;
	do {
		ret = read (0, &buffer[r], G500_PAGE_SIZE-r);
		if (ret == -1) {
			perror ("read");
			return EXIT_FAILURE;
		}
		r += ret;
	} while (r != G500_PAGE_SIZE && ret != 0);
	
	int offset = 0;
	if (argc > 1)
		offset = strtol (argv[1], NULL, 0);

	int i = 2*offset;
	int end = 0;
    int j;
	while (!end) {
		struct g500_macro_item_t *macro_item = (struct g500_macro_item_t *)&buffer[i];
		if (i % 2 == 0)
			printf ("%02X\t", i/2);
		else
			printf ("\t");
		switch (macro_item->type) {
		case G500_MACRO_NOOP:
			printf ("NOOP\n");
			break;
		case G500_MACRO_WAIT_RELEASE:
			printf ("WAIT WHILE PRESSED\n");
			break;
		case G500_MACRO_REPEAT_IF_PRESSED:
			printf ("REPEAT IF PRESSED\n");
			break;
		case G500_MACRO_REPEAT:
			printf ("REPEAT\n");
			break;
		case G500_MACRO_END:
			printf ("END\n");
			end = 1;
			break;
		case G500_MACRO_KEY_PRESS:
			/*if pressed key is letter print it instead of hid code */
			if ((macro_item->value.key_usage > 0x03) && (macro_item->value.key_usage < 0x1E))
				printf ("KEY PRESS %c\n", macro_item->value.key_usage - 0x04 + 0x41);
			else
				printf ("KEY PRESS 0x%02hhX\n", macro_item->value.key_usage);
			break;
		case G500_MACRO_KEY_RELEASE:
			/*if released key is letter print it instead of hid code */
			if ((macro_item->value.key_usage > 0x03) && (macro_item->value.key_usage < 0x1E))
				printf ("KEY RELEASE %c\n", macro_item->value.key_usage - 0x04 + 0x41);
			else
				printf ("KEY RELEASE 0x%02hhX\n", macro_item->value.key_usage);
			break;
		case G500_MACRO_MODIFIER_PRESS:
			printf("MODIFIER PRESS ");
            for(j = 0; j < 8; ++j){
                if (macro_item->value.modifier_bits & (1<<j)){
                    switch(j + 0xE0 - 1){
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
                        default:
                            printf ("0x%02hhX\n", macro_item->value.modifier_bits);

                    }
                }
            }
            printf("\n");
			break;
		case G500_MACRO_MODIFIER_RELEASE:
			printf("MODIFIER RELEASE ");
            for(j = 0; j < 8; ++j){
                if (macro_item->value.modifier_bits & (1<<j)){
                    switch(j + 0xE0 - 1){
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
                        default:
                            printf ("0x%02hhX\n", macro_item->value.modifier_bits);

                    }
                }
            }
            printf("\n");
			break;
		case G500_MACRO_WHEEL:
			printf ("WHEEL %hhd\n", macro_item->value.wheel);
			break;
		case G500_MACRO_BUTTON_PRESS:
			printf ("MOUSE BUTTON PRESS %d\n",
				g500_get_button_num (le16toh (macro_item->value.button)));
			break;
		case G500_MACRO_BUTTON_RELEASE:
			printf ("MOUSE BUTTON RELEASE %d\n",
				g500_get_button_num (le16toh (macro_item->value.button)));
			break;
		case G500_MACRO_CONSUMER_CONTROL:
			printf ("CONSUMER CONTROL %hX\n", be16toh (macro_item->value.cc_usage));
			break;
		case G500_MACRO_DELAY:
			printf ("DELAY %hums\n", be16toh (macro_item->value.delay));
			break;
		case G500_MACRO_JUMP:
			printf ("JUMP 0x%02hhX:0x%02hhX\n",
				macro_item->value.jump.page,
				macro_item->value.jump.offset);
			break;
		case G500_MACRO_JUMP_IF_PRESSED:
			printf ("JUMP IF PRESSED 0x%02hhX:0x%02hhX\n",
				macro_item->value.jump.page,
				macro_item->value.jump.offset);
			break;
		case G500_MACRO_MOUSE_AXES:
			printf ("MOUSE AXES %hd, %hd\n",
				be16toh (macro_item->value.mouse.axis[0]),
				be16toh (macro_item->value.mouse.axis[1]));
			break;
		case G500_MACRO_WAIT_HOLD:
			printf ("WAIT HOLD %hums 0x%02hhX:0x%02hhX\n",
				be16toh (macro_item->value.hold.delay),
				macro_item->value.hold.page,
				macro_item->value.hold.offset);
				break;
		default:
			printf ("UNKNOWN 0x%02hhX\n", macro_item->type);
		}
		i += g500_macro_item_len (macro_item);
	}
	return EXIT_SUCCESS;
}


