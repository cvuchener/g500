0x63 – Resolution
=================

### G500

| Bytes | Type     | Description                                 |
| ----- | -------- | ------------------------------------------- |
| 0–1   | int16 le | X resolution                                |
| 2–3   | int16 le | Y resolution                                |
| 4     | byte     | ? (0 = no change), default is 0x80          |
| 5     | byte     | Angle snap (0 = no change, 1 = off, 2 = on) |

The resolution values are not in dpi, see [resolution.md](../resolution.md) for how to convert them.

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

