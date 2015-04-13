#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <endian.h>
#include <stdarg.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include <linux/hidraw.h>

struct global_t {
	unsigned int usage_page;
	unsigned int report_size;
	unsigned int report_count;
	unsigned int report_id;
};

#define MAX_USAGE 256
struct local_t {
	unsigned int usage[MAX_USAGE];
	int usage_count;
	unsigned int usage_min, usage_max;
};

int parse_report_item (const unsigned char *report, int *type, int *tag, int *data_len, const unsigned char **data);
unsigned int parse_int (const unsigned char *data, int data_len);

int main (int argc, char *argv[]) {
	int i,j;

	if (argc != 2) {
		fprintf (stderr, "Usage: %s hidraw\n", argv[0]);
		return EXIT_FAILURE;
	}

	int fd;
	if (-1 == (fd = open (argv[1], O_RDWR))) {
		perror ("open");
		return EXIT_FAILURE;
	}

	struct hidraw_devinfo info;
	if (-1 == ioctl (fd, HIDIOCGRAWINFO, &info)) {
		perror ("ioctl HIDIOCGRAWINFO");
		return EXIT_FAILURE;
	}

	char name[256];
	if (-1 == ioctl (fd, HIDIOCGRAWNAME(sizeof (name)), name)) {
		perror ("ioctl HIDIOCGRAWNAME");
		return EXIT_FAILURE;
	}

	printf ("%04hx:%04hx %s\n", info.vendor, info.product, name);

        struct hidraw_report_descriptor desc;
        if (-1 == ioctl (fd, HIDIOCGRDESCSIZE, &desc.size)) {
                perror ("ioctl HIDIOCGRDESCSIZE");
                return EXIT_FAILURE;
        }
        if (-1 == ioctl (fd, HIDIOCGRDESC, &desc)) {
                perror ("ioctl HIDIOCGRDESC");
                return EXIT_FAILURE;
        }

	int found_report[2][2] = { { 0, 0 }, { 0, 0 } };
	const int report_ids[2] = { 0x10, 0x11 };
	const int report_counts[2] = { 6, 19 };

#define GLOBAL_STACK_SIZE 16
	struct global_t global_stack[GLOBAL_STACK_SIZE];
	int global_stack_index = 0;
	struct global_t global_state;
	memset (&global_state, 0, sizeof (struct global_t));
	struct local_t local_state;
	memset (&local_state, 0, sizeof (struct local_t));
	
	i = 0;
	while (i < desc.size) {
		int type, tag, data_len;
		const unsigned char *item_data;
		i += parse_report_item (&desc.value[i], &type, &tag, &data_len, &item_data);
		switch (type) {
		case 0: // Main
			switch (tag) {
			case 8: // Input
			case 9: // Output
				if (global_state.usage_page == 0xFF00 && local_state.usage_count == 1 && local_state.usage[0] >= 1 && local_state.usage[0] <= 2) {
					int report_dir = tag == 8 ? 0 : 1;
					int report_type = local_state.usage[0] - 1;
					if (global_state.report_id != report_ids[report_type]) {
						fprintf (stderr, "Invalid report ID\n");
						break;
					}
					if (global_state.report_count != report_counts[report_type]) {
						fprintf (stderr, "Invalid report count\n");
						break;
					}
					if (global_state.report_size != 8) {
						fprintf (stderr, "Invalid report size\n");
						break;
					}
					if (parse_int (item_data, data_len) != 0) {
						fprintf (stderr, "Invalid report flags\n");
						break;
					}
					found_report[report_type][report_dir] = 1;
				}
				break;

			case 11: // Feature
				break;

			case 10: // Collection
			case 12: // End Collection
				break;

			default:
				fprintf (stderr, "Invalid main tag %d\n", tag);
				return EXIT_FAILURE;
			}
			memset (&local_state, 0, sizeof (struct local_t));
			break;

		case 1: // Global
			switch (tag) {
			case 0: // Usage Page
				global_state.usage_page = parse_int (item_data, data_len);
				break;

			case 1: // Logical Minimum
			case 2: // Logical Maximum
			case 3: // Physical Minimum
			case 4: // Physical Maximmum
			case 5: // Unit Exponent
			case 6: // Unit
				break;

			case 7: // Report Size
				global_state.report_size = parse_int (item_data, data_len);
				break;

			case 8: // Report ID
				global_state.report_id = parse_int (item_data, data_len);
				break;

			case 9: // Report Count
				global_state.report_count = parse_int (item_data, data_len);
				break;

			case 10: // Push
				memcpy (&global_state, &global_stack[global_stack_index++], sizeof (struct global_t));
				break;

			case 11: // Pop
				memcpy (&global_stack[--global_stack_index], &global_state, sizeof (struct global_t));
				break;

			default:
				fprintf (stderr, "Invalid global tag %d\n", tag);
				return EXIT_FAILURE;
			}
			break;

		case 2: // Local
			switch (tag) {
			case 0: // Usage
				local_state.usage[local_state.usage_count++] = parse_int (item_data, data_len);
				break;

			case 1: // Usage Minimum
				local_state.usage_min = parse_int (item_data, data_len);
				break;

			case 2: // Usage Maximum 
				local_state.usage_max = parse_int (item_data, data_len);
				break;

			case 3: // Designator Index
			case 4: // Designator Minimum
			case 5: // Designator Maximum
			case 7: // String Index
			case 8: // String Minimum
			case 9: // String Maximum
			case 10: // Delimiter
				break;

			default:
				fprintf (stderr, "Invalid local tag %d\n", tag);
				return EXIT_FAILURE;
			}
			break;

		default:
			fprintf (stderr, "Invalid item type %d\n", type);
			return EXIT_FAILURE;
		}
	}

	for (i = 0; i < 2; ++i) {
		for (j = 0; j < 2; ++j) {
			if (!found_report[i][j]) {
				fprintf (stderr, "Missing logitech HID report\n");
				return EXIT_FAILURE;
			}
		}
	}
	printf ("This device supports Logitech HID reports.\n");
	return EXIT_SUCCESS;
}

int parse_report_item (const unsigned char *report, int *type, int *tag, int *data_len, const unsigned char **data) {
	if (report[0] == 14 ) { // Long item
		*type = report[0];
		*data_len = report[1];
		*tag = report[2];
		*data = &report[3];
		return 3 + *data_len;
	}
	else {
		*data_len = report[0] & 0x03;
		*type = (report[0] & 0x0C) >> 2;
		*tag = (report[0] & 0xF0) >> 4;
		*data = &report[1];
		return 1 + *data_len;
	}
}

unsigned int parse_int (const unsigned char *data, int data_len) {
	switch (data_len) {
	case 0:
		return 0;
	case 1:
		return *data;
	case 2:
		return le16toh (*(uint16_t *)data);
	case 4:
		return le32toh (*(uint32_t *)data);
	default:
		fprintf (stderr, "Invalid int size %d\n", data_len*8);
		return 0;
	}
}
