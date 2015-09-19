Profile for G9
==============

Supported devices:
 - G9 (c048)

Structure
---------

The profile is 78 bytes long.

| Bytes | Type         | Content                        | Comment                  |
| ----- | ------------ | ------------------------------ | ------------------------ |
| 0–2   |              | LED color                      | RGB                      |
| 3     |              | ?                              | 0x00 in 1-4. Profile 5 has 0xFF |
| 4–18  | struct array | 5 3-byte-long DPI modes        |                          |
| 19    | byte         | default DPI mode               | 1-indexed. Profile 5 is special (0x80 + index) |
| 20    | byte         | ?                              | usually 0x21             |
| 21    | byte         | ?                              | usually 0xa2             |
| 22    | byte         | USB refresh rate               | fraction of 1000Hz       |
| 23–52 | struct array | 10 3-byte-long button bindings |                          |
| 53–55 |              | ?                              | 0x8f0000 for 1-4. 0xFFFFFF for profile 5 |

### DPI mode

The G9 can go from 200 to 3200 dpi in steps of 200.
For each DPI mode, the structure is:

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0     | byte       | resolution         | First bit is always 1. Rest is dpi * 200. 0x00 mean off |
| 1–2   | int16 le   | LEDs status        | each half-byte is a LED: 1: off, 2: on  |

Only the first byte is null when the mode is off.

For the LEDs status, each half-byte from lowest to highest corresponds to:
 - High
 - Mid
 - Low
 - “Running man” icon
-
