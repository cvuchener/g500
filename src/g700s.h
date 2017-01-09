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

#ifndef G700S_H
#define G700S_H

#include <stdint.h>
#include <stddef.h>

/* use logitech_query with one of these values and the corresponding g700s_*_params_t */
#define G700S_QUERY_TYPE_PROFILE	0x0F // ???
#define G700S_QUERY_TYPE_LEDS	0x51
#define G700S_QUERY_TYPE_RESOLUTION	0x63
#define G700S_QUERY_TYPE_REFRESH_RATE	0x64
#define G700S_QUERY_TYPE_MEMORY_OP	0xA0
#define G700S_QUERY_TYPE_SEQ_NUM	0xA1 // ???
#define G700S_QUERY_TYPE_MEMORY	0xA2

#define G700S_PAGE_SIZE 	512

/*
* Profile
*/

struct __attribute__ ((__packed__)) g700s_profile_params_t {
	uint8_t unk1; // valid values are 0, 1, 2, 3 or FF
	uint8_t profile_page;
	uint8_t unk2; // unused?
};

int g700s_disable_profile (int fd);
int g700s_use_default_profile (int fd);
int g700s_use_profile (int fd, uint8_t profile_page);

/*
* LEDs
*/

#define G700S_LED_HIGH_OFF	0x0001
#define G700S_LED_HIGH_ON	0x0002
#define G700S_LED_MID_OFF	0x0010
#define G700S_LED_MID_ON		0x0020
#define G700S_LED_LOW_OFF	0x0100
#define G700S_LED_LOW_ON		0x0200
#define G700S_LED_LOGO_OFF	0x1000
#define G700S_LED_LOGO_ON	0x2000

struct __attribute__ ((__packed__)) g700s_leds_params_t {
	uint16_t leds; // little endian
	uint8_t unknown; // unused?
};

int g700s_get_leds (int fd);
int g700s_set_leds (int fd, int leds);

/*
* Resolution
*/

struct __attribute__ ((__packed__)) g700s_resolution_params_t {
	uint8_t resolution[2]; // little endian
	uint8_t unknown[2]; // 80 01 even after trying to modify it
	uint8_t padding[10]; // not used, not initialized when getting
};

/* pack/unpack resolution do not resolve endianess as it is different from memory (be) and query (le) */
uint8_t g700s_pack_resolution (int resolution);
int g700s_unpack_resolution (uint8_t resolution);

int g700s_get_resolution (int fd, int resolution[1]);
int g700s_set_resolution (int fd, const int resolution[2]);

/*
* Refresh rate
*/

struct __attribute__ ((__packed__)) g700s_refresh_rate_params_t {
	uint8_t refresh_rate;
	uint8_t unknown[2]; // unused ?
};

uint8_t g700s_pack_refresh_rate (int refresh_rate);
int g700s_unpack_refresh_rate (uint8_t refresh_rate);

int g700s_get_refresh_rate (int fd);
int g700s_set_refresh_rate (int fd, int refresh_rate);

/*
* Memory operations
*/

#define G700S_MEM_OP_FILL	0x02
#define G700S_MEM_OP_AND	0x03

struct __attribute__ ((__packed__)) g700s_mem_op_params_t {
	uint8_t op;
	uint8_t unk1[1];
	uint8_t src_offset; // used by op==4
	uint8_t src_len; // used by op==4
	uint8_t unk2[2];
	uint8_t page;
	uint8_t offset;
	uint8_t unk3[2];
	uint16_t len; // big endian
	uint8_t unk4[4];
};

/*
* Memory read
*/

struct __attribute__ ((__packed__)) g700s_memory_read_params_t {
	uint8_t page;
	uint8_t offset;
	uint8_t unknown; // unused?
};

int g700s_read_mem (int fd, uint8_t page, uint8_t offset, void *dest, size_t len);

/*
* Memory write
*/

#define G700S_DATA	0x90
#define G700S_DATA_FLAG_CONTINUE	0x01
#define G700S_DATA_FLAG_ACK	0x02

#define G700S_DATA_ACK	0x50

struct __attribute__ ((__packed__)) g700s_data_header_t {
	uint8_t unk1; // must be 0x01
	uint8_t page;
	uint8_t offset;
	uint8_t unk2[2];
	uint16_t len; // big endian
	uint8_t unk3[2];
};

/**
* Low-level send data with type \p type.
*
* data may need to contain a header.
*/
int g700s_send_data (int fd, uint8_t type, uint8_t seq_num, const uint8_t *data);
/**
* Send has much data as it can with only one message.
*
* Returns the number of bytes sent.
*/
int g700s_write_some (int fd, const struct g700s_data_header_t *header, uint8_t seq_num, const void *data, size_t len);
/**
* Send len bytes in data.
*
* Reset the sequence number and send several messages.
*/
int g700s_write_page (int fd, uint8_t page, uint8_t offset, const void *data, size_t len);

/*
* Profile
*/

#define G700S_BINDING_MACRO_MAX 0x7F // Actually max page number is 0x12, but bit 7 seems to be the difference between macro and the rest
#define G700S_BINDING_MOUSE_BUTTON	0x81
#define G700S_BINDING_KEY	0x82
#define G700S_BINDING_SPECIAL_BUTTON	0x83
#define G700S_BINDING_CONSUMER_CONTROL	0x84
#define G700S_BINDING_UNSET	0x8F

struct __attribute__ ((__packed__)) g700s_profile_t {
	struct __attribute__ ((__packed__)) {
		uint8_t resolution[2]; // big endian
		uint16_t leds; // little endian
	} dpi_mode[5];
	uint8_t default_mode;
	uint8_t angle;
	uint8_t angle_snap;
	uint8_t acceleration;
	uint8_t refresh_rate;
	uint8_t unk[10];



	struct __attribute__ ((__packed__)) {
		uint8_t type;
		union {
			uint8_t bytes[2];
			uint16_t button; // little endian
			struct __attribute__ ((__packed__)) {
				uint8_t modifier;
				uint8_t usage;
			} key;
			uint16_t cc_usage; // big endian
			struct __attribute__ ((__packed__)) {
				uint8_t offset;
				uint8_t unused;
			} macro;
		} value;
	} binding[13];
};

#define G700S_SPECIAL_BUTTON_PAN_LEFT	0x0001
#define G700S_SPECIAL_BUTTON_PAN_RIGHT	0x0002
#define G700S_SPECIAL_BUTTON_BATTERY 	0x0003
#define G700S_SPECIAL_BUTTON_DPI_PLUS	0x0004
#define G700S_SPECIAL_BUTTON_DPI_MINUS	0x0008
#define G700S_SPECIAL_BUTTON_DPI_CYC_DOWN    0x0009
#define G700S_SPECIAL_BUTTON_PROFILE_CYC	0x0011
#define G700S_SPECIAL_BUTTON_DPI_CYC_UP    0x0105

/* Returns the button number from the bitfield. */
int g700s_get_button_num (uint16_t bits);

/* Prints user friendly mouse button identifier */
const char *g700s_button_name(int number);

/*
* Macro
*/

#define G700S_MACRO_NOOP	0x00
#define G700S_MACRO_WAIT_RELEASE	0x01
#define G700S_MACRO_REPEAT_IF_PRESSED 0x02
#define G700S_MACRO_REPEAT	0x03
#define G700S_MACRO_KEY_PRESS	0x20
#define G700S_MACRO_KEY_RELEASE	0x21
#define G700S_MACRO_MODIFIER_PRESS	0x22
#define G700S_MACRO_MODIFIER_RELEASE	0x23
#define G700S_MACRO_WHEEL	0x24
#define G700S_MACRO_BUTTON_PRESS	0x40
#define G700S_MACRO_BUTTON_RELEASE	0x41
#define G700S_MACRO_CONSUMER_CONTROL	0x42
#define G700S_MACRO_DELAY	0x43
#define G700S_MACRO_JUMP	0x44
#define G700S_MACRO_JUMP_IF_PRESSED	0x45
#define G700S_MACRO_MOUSE_AXES	0x60
#define G700S_MACRO_WAIT_HOLD	0x61
#define G700S_MACRO_END	0xFF

struct __attribute__ ((__packed__)) g700s_macro_item_t {
	uint8_t type;
	union {
		uint8_t key_usage;
		uint8_t modifier_bits;
		int8_t wheel;
		uint16_t button;
		uint16_t cc_usage;
		uint16_t delay;
		struct __attribute__ ((__packed__)) {
			uint8_t page;
			uint8_t offset;
		} jump;
		struct __attribute__ ((__packed__)) {
			uint16_t axis[2];
		} mouse;
		struct __attribute__ ((__packed__)) {
			uint16_t delay;
			uint8_t page;
			uint8_t offset;
		} hold;
	} value;
};

/* Get the real size for the macro item */
size_t g700s_macro_item_len (const struct g700s_macro_item_t *item);

#endif /* !defined G700S_H */
