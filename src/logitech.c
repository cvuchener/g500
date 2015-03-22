#include "logitech.h"

#include <unistd.h>
#include <string.h>
#include <endian.h>

#ifdef DEBUG
#include <stdio.h>
#endif

size_t logitech_get_report_len (uint8_t type) {
	if (type == LOGITECH_REPORT_SHORT)
		return 6;
	else if (type == LOGITECH_REPORT_LONG)
		return 19;
	return -1;
}


int logitech_report_out (int fd, const struct logitech_report_t *report) {
	size_t len;

	if (-1 == (len = logitech_get_report_len (report->id)))
		return -1;

#ifdef DEBUG_PRINT_REPORT
	{
		fprintf (stderr, "Send %02hhX", report->id);
		int i;
		for (i = 0; i < len; ++i)
			fprintf (stderr, " %02hhX", report->data[i]);
		fprintf (stderr, "\n");
	}
#endif

	if (-1 == write (fd, report, len+1)) {
#ifdef DEBUG
		perror ("write");
#endif
		return -1;
	}
	return 0;
}

int logitech_report_in (int fd, struct logitech_report_t *report) {
	int ret;
	size_t len;
	if (-1 == (ret = read (fd, report, sizeof (struct logitech_report_t)))) {
#ifdef DEBUG
		perror ("write");
#endif
		return -1;
	}
	if (ret == 0) {
#ifdef DEBUG
		fprintf (stderr, "Read end of file\n");
#endif
		return -1;
	}

#ifdef DEBUG_PRINT_REPORT
	{
		fprintf (stderr, "Recv %02hhX", report->id);
		int i;
		for (i = 0; i < ret-1; ++i)
			fprintf (stderr, " %02hhX", report->data[i]);
		fprintf (stderr, "\n");
	}
#endif

	if (-1 == (len = logitech_get_report_len (report->id))) {
#ifdef DEBUG
		fprintf (stderr, "Invalid report id %d\n", report->id);
#endif
		return -1;
	}
	if (ret != len+1) {
#ifdef DEBUG
		fprintf (stderr, "Invalid answer length %d\n", ret);
#endif
		return -1;
	}

	return 0;
}

int logitech_is_error_report (const struct logitech_report_t *report, uint8_t *query_code, uint16_t *error_code) {
	if (report->id == LOGITECH_REPORT_SHORT && report->data[0] == 0x00 && report->data[1] == LOGITECH_REPORT_ERROR) {
		if (query_code)
			*query_code = report->data[2];
		if (error_code)
			*error_code = le16toh (*(uint16_t *)&report->data[4]);
		return 1;
	}
	return 0;
}

int logitech_query (int fd, uint8_t query, uint8_t type, const uint8_t *params, uint8_t *result) {
	struct logitech_report_t report_out, report_in;
	uint8_t expected_in_report_type;
	size_t params_len, result_len;
	switch (query) {
	case LOGITECH_READ_SHORT:
	case LOGITECH_SEND_SHORT:
		report_out.id = LOGITECH_REPORT_SHORT;
		expected_in_report_type = LOGITECH_REPORT_SHORT;
		break;
	case LOGITECH_READ_LONG:
		report_out.id = LOGITECH_REPORT_SHORT;
		expected_in_report_type = LOGITECH_REPORT_LONG;
		break;
	case LOGITECH_SEND_LONG:
		report_out.id = LOGITECH_REPORT_LONG;
		expected_in_report_type = LOGITECH_REPORT_SHORT;
		break;
	default:
		return -1;
	}
	params_len = logitech_get_report_len (report_out.id) - 3;
	
	report_out.data[0] = 0x00;
	report_out.data[1] = query;
	report_out.data[2] = type;
	if (params)
		memcpy (&report_out.data[3], params, params_len);
	else
		memset (&report_out.data[3], 0, params_len);
	
	if (-1 == logitech_report_out (fd, &report_out)) 
		return -1;

	if (-1 == logitech_report_in (fd, &report_in)) 
		return -1;

	uint16_t error_code;
	if (logitech_is_error_report (&report_in, NULL, &error_code)) {
#ifdef DEBUG
		fprintf (stderr, "Received error code %d\n", error_code);
#endif
		return -1;
	}
	if (report_in.id != expected_in_report_type || report_in.data[1] != query || report_in.data[2] != type) {
#ifdef DEBUG
		fprintf (stderr, "Invalid answer\n");
#endif
		return -1;
	}

	if (result) {
		result_len = logitech_get_report_len (expected_in_report_type) - 3;
		memcpy (result, &report_in.data[3], result_len);
	}
	return 0;
}

