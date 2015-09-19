0x63 – Resolution
=================

### G500

The resolution is encoded in two 16 bits integer, one for the X axis and one for the Y axis. The value are not in dpi, for the G500, 17 units correspond to 400 dpi; for the G500s, 1 unit corresponds to 50 dpi.

The data size for these queries is 6 bytes: 4 bytes for the two resolution value in little-endian and 2 bytes for unknown purposes. Writing those 2 bytes does not seem to change anything. When reading they usually are 80 01, but they can also be FF FF for an invalid profile. The rest of the bytes are uninitialized (they have the same value as in the last long report from the device).

Example on the G500:
```
out  10 00 83 63 00 00 00
in   11 00 83 63 22 00 22 00 80 01 00 00 00 00 00 00 00 00 00 00
```
The current resolution is 400 dpi (0x0022).
```
out  11 00 82 63 F3 00 F3 00 00 00 00 00 00 00 00 00 00 00 00 00
in   10 00 82 63 00 00 00
```
Set the current resolution to 5700 dpi (0x00F3).
```
out  10 00 83 63 00 00 00
in   11 00 83 63 F3 00 F3 00 80 01 00 00 00 00 00 00 00 00 00 00
```
The current resolution is now 5700 dpi.

Higher resolution than the maximum (5700 dpi = 0x00F3 for the G500, 8200 dpi = 0x00A4 for the G500s) will not generate any error but they will not be faster.


### G5

The resolution is a single byte.

| Value | Resolution |
| ----- | ---------- |
| 0x80  | 400 dpi    |
| 0x81  | 800 dpi    |
| 0x82  | 1600 dpi   |
| 0x83  | 2000 dpi   |

