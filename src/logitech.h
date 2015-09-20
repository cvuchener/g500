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

#ifndef LOGITECH_H
#define LOGITECH_H

#include <stdint.h>
#include <stddef.h>

#define LOGITECH_REPORT_SHORT	0x10
#define LOGITECH_REPORT_LONG	0x11

struct __attribute__ ((__packed__)) logitech_report_t {
	uint8_t id;
	uint8_t data[19];
};

/**
 * Get report length from type.
 */
size_t logitech_get_report_len (uint8_t type);

/**
 * Send an output report.
 *
 * Returns -1 if an error occurred.
 */
int logitech_report_out (int fd, const struct logitech_report_t *report);
/**
 * Receive an input report.
 *
 * Returns -1 if an error occurred.
 */
int logitech_report_in (int fd, struct logitech_report_t *report);

#define LOGITECH_REPORT_ERROR	0x8F

#define LOGITECH_ERROR_INVALID_SUBID	0x01
#define LOGITECH_ERROR_INVALID_ADDRESS	0x02
#define LOGITECH_ERROR_INVALID_VALUE	0x03
#define LOGITECH_ERROR_CONNECT_FAIL	0x04
#define LOGITECH_ERROR_TOO_MANY_DEVICES	0x05
#define LOGITECH_ERROR_ALREADY_EXISTS	0x06
#define LOGITECH_ERROR_BUSY	0x07
#define LOGITECH_ERROR_UNKNOWN_DEVICE	0x08
#define LOGITECH_ERROR_RESOURCE_ERROR	0x09
#define LOGITECH_ERROR_REQUEST_UNAVAILABLE	0x0A
#define LOGITECH_ERROR_INVALID_PARAM_VALUE	0x0B
#define LOGITECH_ERROR_WRONG_PIN_CODE	0x0C
#define LOGITECH_ERROR_MAX	0x0C

extern const char *logitech_error_string[LOGITECH_ERROR_MAX+1];

/**
 * Test if report is an error report. Set query_code and error_code to the value in the error report.
 *
 * Return non-zero if report is an error report.
 */
int logitech_is_error_report (const struct logitech_report_t *report, uint8_t *query_code, uint8_t *error_code);

#define LOGITECH_SEND_SHORT	0x80
#define LOGITECH_READ_SHORT	0x81
#define LOGITECH_SEND_LONG	0x82
#define LOGITECH_READ_LONG	0x83

/**
 * Send a query to the device.
 *
 * If params is null, the output report parameter section is filled with zero.
 * If data is non-null, it is filled with the data returned from the device.
 *
 * Returns -1 if an error occurred.
 */
int logitech_query (int fd, uint8_t query, uint8_t type, const uint8_t *params, uint8_t *data);

#endif /* !defined LOGITECH_H */
