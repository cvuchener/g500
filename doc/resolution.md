Resolution
==========

Data format
-----------

### G5

The resolution is a single byte.

| Value | Resolution |
| ----- | ---------- |
| 0x80  | 400 dpi    |
| 0x81  | 800 dpi    |
| 0x82  | 1600 dpi   |
| 0x83  | 2000 dpi   |


### G9

The resolution is a single byte: 0x80 + resolution ÷ 200.

| Value | Resolution |
| ----- | ---------- |
| 0x81  | 200 dpi    |
| 0x82  | 400 dpi    |
| 0x83  | 600 dpi    |
| 0x84  | 800 dpi    |
| 0x85  | 1000 dpi   |
| 0x86  | 1200 dpi   |
| 0x87  | 1400 dpi   |
| 0x88  | 1600 dpi   |
| 0x89  | 1800 dpi   |
| 0x8a  | 2000 dpi   |
| 0x8b  | 2200 dpi   |
| 0x8c  | 2400 dpi   |
| 0x8d  | 2600 dpi   |
| 0x8e  | 2800 dpi   |
| 0x8f  | 2000 dpi   |
| 0x90  | 3200 dpi   |


### G500

The resolution is two 16 bits integer (X and Y axes).

17 units correspond to 400 dpi.

Highest resolution is 0x00F3 (5700 dpi).


### G500s, G700s

The resolution is two 16 bits integer (X and Y axes).

1 unit corresponds to 50 dpi.

Highest resolution is 0x00A4 (8200 dpi).
