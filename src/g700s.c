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

#include "g700s.h"

#include <string.h>
#include <endian.h>
#include "logitech.h"

#ifdef DEBUG
#include <stdio.h>
#endif

int g700s_disable_profile (int fd) {
	struct g700s_profile_params_t params;
	memset (&params, 0, sizeof (params));
	params.unk1 = 0xFF;
	if (0 != logitech_query (fd, LOGITECH_SEND_SHORT, G700S_QUERY_TYPE_PROFILE, (uint8_t *)&params, NULL))
		return -1;
	return 0;
}

int g700s_use_default_profile (int fd) {
	struct g700s_profile_params_t params;
	memset (&params, 0, sizeof (params));
	if (0 != logitech_query (fd, LOGITECH_SEND_SHORT, G700S_QUERY_TYPE_PROFILE, (uint8_t *)&params, NULL))
		return -1;
	return 0;
}

int g700s_use_profile (int fd, uint8_t profile_page) {
	struct g700s_profile_params_t params;
	memset (&params, 0, sizeof (params));
	params.unk1 = 0x01;
	params.profile_page = profile_page;
	if (0 != logitech_query (fd, LOGITECH_SEND_SHORT, G700S_QUERY_TYPE_PROFILE, (uint8_t *)&params, NULL))
		return -1;
	return 0;
}

int g700s_get_leds (int fd) {
	struct g700s_leds_params_t params;
	if (0 != logitech_query (fd, LOGITECH_READ_SHORT, G700S_QUERY_TYPE_LEDS, NULL, (uint8_t *)&params))
		return -1;
	return le16toh (params.leds);
}

int g700s_set_leds (int fd, int leds) {
	struct g700s_leds_params_t params;
	params.leds = htole16 (leds);
	params.unknown = 0;
	if (0 != logitech_query (fd, LOGITECH_SEND_SHORT, G700S_QUERY_TYPE_LEDS, (uint8_t *)&params, NULL))
		return -1;
	return 0;
}

uint8_t g700s_pack_resolution (int resolution) {
	return resolution/50;
}

int g700s_unpack_resolution (uint8_t resolution) {
	//printf("\nunpacking value %x\n",resolution);
	return resolution*50;
}

int g700s_get_resolution (int fd, int resolution[2]) {
	struct g700s_resolution_params_t params;
	if (0 != logitech_query (fd, LOGITECH_READ_LONG, G700S_QUERY_TYPE_RESOLUTION, NULL, (uint8_t *)&params))
		return  -1;
	int i;
	for (i = 0; i < 2; ++i)
		resolution[i] = g700s_unpack_resolution (le16toh (params.resolution[i]));
	return 0;
}

int g700s_set_resolution (int fd, const int resolution[2]) {
	struct g700s_resolution_params_t params;
	int i;
	for (i = 0; i < 2; ++i)
		params.resolution[i] = htole16 (g700s_pack_resolution (resolution[i]));
	params.unknown[0] = params.unknown[1] = 0;
	if (0 != logitech_query (fd, LOGITECH_SEND_LONG, G700S_QUERY_TYPE_RESOLUTION, (uint8_t *)&params, NULL))
		return -1;
	return 0;
}

uint8_t g700s_pack_refresh_rate (int refresh_rate) {
	return 1000/refresh_rate;
}

int g700s_unpack_refresh_rate (uint8_t refresh_rate) {
	return 1000/refresh_rate;
}

int g700s_get_refresh_rate (int fd) {
	struct g700s_refresh_rate_params_t params;
	if (0 != logitech_query (fd, LOGITECH_READ_SHORT, G700S_QUERY_TYPE_REFRESH_RATE, NULL, (uint8_t *)&params))
		return -1;
	return g700s_unpack_refresh_rate (params.refresh_rate);
}

int g700s_set_refresh_rate (int fd, int refresh_rate) {
	struct g700s_refresh_rate_params_t params;
	params.refresh_rate = g700s_pack_refresh_rate (refresh_rate);
	if (0 != logitech_query (fd, LOGITECH_SEND_SHORT, G700S_QUERY_TYPE_REFRESH_RATE, (uint8_t *)&params, NULL))
		return -1;
	return 0;
}

int g700s_read_mem (int fd, uint8_t page, uint8_t offset, void *dest, size_t len) {
	uint8_t buffer[16];
	struct g700s_memory_read_params_t params;
	int i = 0;
	while (i < len) {
		params.page = page;
		params.offset = offset + i/2;
		params.unknown = 0;
		if (0 != logitech_query (fd, LOGITECH_READ_LONG, G700S_QUERY_TYPE_MEMORY, (uint8_t *)&params, buffer))
			return -1;
		memcpy (dest+i, buffer, len-i < 16 ? len-i : 16);
		i += 16;
	}
	return 0;
}

int g700s_send_data (int fd, uint8_t type, uint8_t seq_num, const uint8_t *data) {
	struct logitech_report_t report_out, report_in;

	report_out.id = LOGITECH_REPORT_LONG;
	report_out.data[0] = 0x00;
	report_out.data[1] = type;
	report_out.data[2] = seq_num;
	memcpy (&report_out.data[3], data, 16);


	if (-1 == logitech_report_out (fd, &report_out))
		return -1;

	if (type & G700S_DATA_FLAG_ACK) {
		if (-1 == logitech_report_in (fd, &report_in))
			return -1;
		
		uint8_t error_code;
		if (logitech_is_error_report (&report_in, NULL, &error_code)) {
#ifdef DEBUG
			fprintf (stderr, "Received error code %d\n", error_code);
#endif
			return -1;
		}
		
		if (report_in.data[1] != G700S_DATA_ACK) {
#ifdef DEBUG
			fprintf (stderr, "Wrong type of answer %d\n", report_in.data[1]);
#endif
			return -1;
		}

		if (report_in.data[2] != 1) {
#ifdef DEBUG
			fprintf (stderr, "Acknowledgment error %d\n", report_in.data[2]);
#endif
			return -1;
		}

		if (report_in.data[3] != seq_num) {
#ifdef DEBUG
			fprintf (stderr, "Wrong sequence number in acknowledgment\n");
#endif
			return -1;
		}
	}

	return 0;
}

int g700s_write_some (int fd, const struct g700s_data_header_t *header, uint8_t seq_num, const void *data, size_t len) {
	int w;

	uint8_t type = G700S_DATA | G700S_DATA_FLAG_ACK;
	uint8_t buffer[16];

	int data_begin = 0;
	if (header) {
		size_t header_len = sizeof (struct g700s_data_header_t);
		memcpy (&buffer[0], header, header_len);
		data_begin = header_len;
	}
	else {
		type |= G700S_DATA_FLAG_CONTINUE;
	}

	if (len < 16 - data_begin) {
		memcpy (&buffer[data_begin], data, len);
		memset (&buffer[data_begin + len], 0xFF, 16 - data_begin - len);
		w = len;
	}
	else {
		memcpy (&buffer[data_begin], data, 16 - data_begin);
		w = 16 - data_begin;
	}

	if (-1 == g700s_send_data (fd, type, seq_num, buffer)) {
		return -1;
	}

	return w;
}

int g700s_write_page (int fd, uint8_t page, uint8_t offset, const void *data, size_t len) {
	int i;
	int seq_num = 0;

	/* Reset seq num */
	if (0 != logitech_query (fd, LOGITECH_SEND_SHORT, G700S_QUERY_TYPE_SEQ_NUM, (uint8_t[]) { 0x01, 0, 0 }, NULL)) {
#ifdef DEBUG
		fprintf (stderr, "Could not reset sequence number\n");
#endif
		return -1;
	}
	
	struct g700s_data_header_t header;
	memset (&header, 0, sizeof (struct g700s_data_header_t));
	header.unk1 = 1;
	header.page = page;
	header.offset = offset;
	header.len = htobe16 (len);

	if (-1 == (i = g700s_write_some (fd, &header, seq_num++, data, len))) {
#ifdef DEBUG
		fprintf (stderr, "Failed sending data\n");
#endif
		return -1;
	}
	while  (i < len) {
		int ret;
		if (-1 == (ret = g700s_write_some (fd, NULL, seq_num++, data+i, len-i))) {
#ifdef DEBUG
			fprintf (stderr, "Failed sending data\n");
#endif
			return -1;
		}
		i += ret;
	}
	return 0;
}

int g700s_get_button_num (uint16_t bits) {
	int i;
	for (i = 0; i < 16; ++i) {
		if (bits & (1<<i)) {
			return i;
		}
	}
	return -1;
}

size_t g700s_macro_item_len (const struct g700s_macro_item_t *item) {
	if ((item->type & 0xF0) == 0x20)
		return 2;
	else if ((item->type & 0xF0) == 0x40)
		return 3;
	else if ((item->type & 0xF0) == 0x60)
		return 5;
	else
		return 1;
}
const char *g700s_button_name(int number){
    static char G_button[5];
	if (number == 0)
		return "Left   button";
	if (number == 1)
		return "Right  button";
	if (number == 2)
		return "Middle button";
	if (number < 11){
        snprintf(G_button, sizeof(G_button), "G %2d", number+1);
		return G_button;
    }
	if (number == 11)
		return "Wheel tilt left";
	if (number == 12)
		return "Wheel tilt right";
	else
		return "Wrong button nr";
}






