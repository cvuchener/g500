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

/**
 * Test if report is an error report. Set query_code and error_code to the value in the error report.
 *
 * Return non-zero if report is an error report.
 */
int logitech_is_error_report (const struct logitech_report_t *report, uint8_t *query_code, uint16_t *error_code);

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
