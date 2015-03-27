#ifndef G500_H
#define G500_H

#include <stdint.h>
#include <stddef.h>

/* use logitech_query with one of these values and the corresponding g500_*_params_t */
#define G500_QUERY_TYPE_PROFILE	0x0F // ???
#define G500_QUERY_TYPE_LEDS	0x51
#define G500_QUERY_TYPE_RESOLUTION	0x63
#define G500_QUERY_TYPE_REFRESH_RATE	0x64
#define G500_QUERY_TYPE_MEMORY_OP	0xA0
#define G500_QUERY_TYPE_SEQ_NUM	0xA1 // ???
#define G500_QUERY_TYPE_MEMORY	0xA2

#define G500_PAGE_SIZE 	512

/*
 * Profile
 */

struct __attribute__ ((__packed__)) g500_profile_params_t {
	uint8_t unk1; // valid values are 0, 1, 2, 3 or FF
	uint8_t profile_page;
	uint8_t unk2; // unused?
};

int g500_disable_profile (int fd);
int g500_use_default_profile (int fd);
int g500_use_profile (int fd, uint8_t profile_page);

/*
 * LEDs
 */

#define G500_LED_HIGH_OFF	0x0001
#define G500_LED_HIGH_ON	0x0002
#define G500_LED_MID_OFF	0x0010
#define G500_LED_MID_ON		0x0020
#define G500_LED_LOW_OFF	0x0100
#define G500_LED_LOW_ON		0x0200
#define G500_LED_LOGO_OFF	0x1000
#define G500_LED_LOGO_ON	0x2000

struct __attribute__ ((__packed__)) g500_leds_params_t {
	uint16_t leds; // little endian
	uint8_t unknown; // unused?
};

int g500_get_leds (int fd);
int g500_set_leds (int fd, int leds);

/*
 * Resolution
 */

struct __attribute__ ((__packed__)) g500_resolution_params_t {
	uint16_t resolution[2]; // little endian
	uint8_t unknown[2]; // 80 01 even after trying to modify it
	uint8_t padding[10]; // not used, not initialized when getting
};

/* pack/unpack resolution do not resolve endianess as it is different from memory (be) and query (le) */
uint16_t g500_pack_resolution (int resolution);
int g500_unpack_resolution (uint16_t resolution);

int g500_get_resolution (int fd, int resolution[2]);
int g500_set_resolution (int fd, const int resolution[2]);

/*
 * Refresh rate
 */

struct __attribute__ ((__packed__)) g500_refresh_rate_params_t {
	uint8_t refresh_rate;
	uint8_t unknown[2]; // unused ?
};

uint8_t g500_pack_refresh_rate (int refresh_rate);
int g500_unpack_refresh_rate (uint8_t refresh_rate);

int g500_get_refresh_rate (int fd);
int g500_set_refresh_rate (int fd, int refresh_rate);

/*
 * Memory operations
 */

#define G500_MEM_OP_FILL	0x02
#define G500_MEM_OP_AND	0x03

struct __attribute__ ((__packed__)) g500_mem_op_params_t {
	uint8_t op;
	uint8_t unk1[1];
	uint8_t src_offset; // used by op==4
	uint8_t src_len; // used by op==4
	uint8_t unk2[2];
	uint8_t page;
	uint8_t offset;
	uint8_t unk3[2];
	uint16_t len; // big enddian
	uint8_t unk4[4];
};

/*
 * Memory read
 */

struct __attribute__ ((__packed__)) g500_memory_read_params_t {
	uint8_t page;
	uint8_t offset;
	uint8_t unknown; // unused?
};

int g500_read_mem (int fd, uint8_t page, uint8_t offset, void *dest, size_t len);

/*
 * Memory write
 */

#define G500_DATA	0x90
#define G500_DATA_FLAG_CONTINUE	0x01
#define G500_DATA_FLAG_ACK	0x02

#define G500_DATA_ACK	0x50

struct __attribute__ ((__packed__)) g500_data_header_t {
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
int g500_send_data (int fd, uint8_t type, uint8_t seq_num, const uint8_t *data);
/**
 * Send has much data as it can with only one message.
 *
 * Returns the number of bytes sent.
 */
int g500_write_some (int fd, const struct g500_data_header_t *header, uint8_t seq_num, const void *data, size_t len);
/**
 * Send len bytes in data.
 *
 * Reset the sequence number and send several messages.
 */
int g500_write_page (int fd, uint8_t page, uint8_t offset, const void *data, size_t len);

/*
 * Profile
 */

#define G500_BINDING_MACRO_MAX 0x7F // Actually max page number is 0x12, but bit 7 seems to be the difference between macro and the rest
#define G500_BINDING_MOUSE_BUTTON	0x81
#define G500_BINDING_KEY	0x82
#define G500_BINDING_SPECIAL_BUTTON	0x83
#define G500_BINDING_CONSUMER_CONTROL	0x84
#define G500_BINDING_UNSET	0x8F

struct __attribute__ ((__packed__)) g500_profile_t {
	uint8_t unk1[4];
	struct __attribute__ ((__packed__)) {
		uint16_t resolution[2]; // big endian
		uint16_t leds; // little endian
	} dpi_mode[5];
	uint16_t angle;
	uint8_t unk2[2];
	uint8_t refresh_rate;
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

#define G500_SPECIAL_BUTTON_PAN_LEFT	0x0001
#define G500_SPECIAL_BUTTON_PAN_RIGHT	0x0002
#define G500_SPECIAL_BUTTON_DPI_PLUS	0x0004
#define G500_SPECIAL_BUTTON_DPI_MINUS	0x0008

/* Returns the button number from the bitfield. */
int g500_get_button_num (uint16_t bits);

/*
 * Macro
 */

#define G500_MACRO_NOOP	0x00
#define G500_MACRO_WAIT_RELEASE	0x01
#define G500_MACRO_REPEAT_IF_PRESSED 0x02
#define G500_MACRO_REPEAT	0x03
#define G500_MACRO_KEY_PRESS	0x20
#define G500_MACRO_KEY_RELEASE	0x21
#define G500_MACRO_MODIFIER_PRESS	0x22
#define G500_MACRO_MODIFIER_RELEASE	0x23
#define G500_MACRO_WHEEL	0x24
#define G500_MACRO_BUTTON_PRESS	0x40
#define G500_MACRO_BUTTON_RELEASE	0x41
#define G500_MACRO_CONSUMER_CONTROL	0x42
#define G500_MACRO_DELAY	0x43
#define G500_MACRO_JUMP	0x44
#define G500_MACRO_JUMP_IF_PRESSED	0x45
#define G500_MACRO_MOUSE_AXES	0x60
#define G500_MACRO_WAIT_HOLD	0x61
#define G500_MACRO_END	0xFF

struct __attribute__ ((__packed__)) g500_macro_item_t {
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
size_t g500_macro_item_len (const struct g500_macro_item_t *item);

#endif /* !defined G500_H */
