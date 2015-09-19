Profile for G500
================

Supported devices:
 - G500 (c068)
 - G500s (c24e)

Structure
---------

The profile is 78 bytes long.

| Bytes | Type         | Content                        | Comment                  |
| ----- | ------------ | ------------------------------ | ------------------------ |
| 0–3   |              | ?                              | usually `FF 00 00 80` but may vary |
| 4–33  | struct array | 5 6-byte-long DPI modes        |                          |
| 34    | byte         | angle correction               | 1: off, 2: on            |
| 35    | byte         | default DPI mode               | current mode when plugging the mouse or loading profile |
| 36    | byte         | some kind of movement filter   | acceleration? default is 0x10 |
| 37    | byte         | ?                              | usually 0x10             |
| 38    | byte         | USB refresh rate               | fraction of 1000Hz       |
| 39–77 | struct array | 13 3-byte-long button bindings |                          |

### DPI mode

For each DPI mode, the structure is:

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0–1   | int16 be   | X resolution       | dpi × 17 ÷ 400           |
| 2–3   | int16 be   | Y resolution       | dpi × 17 ÷ 400           |
| 4–5   | int16 le   | LEDs status        | each half-byte is a LED: 1: off, 2: on  |

All bytes are null when the mode is disabled.

For the LEDs status, each half-byte from lowest to highest corresponds to:
 - High
 - Mid
 - Low
 - “Running man” icon

### Buttons

| Button | G500, G500s      |
| ------ | ---------------- |
| 0      | Left button      |
| 1      | Right button     |
| 2      | Wheel button     |
| 3      | Thumb back       |
| 4      | Thumb forward    |
| 5      | Thumb middle     |
| 6      | Wheel tilt left  |
| 7      | Wheel tilt right |
| 8      | DPI +            |
| 9      | DPI −            |
| 10     | N/A              |
| 11     | N/A              |
| 12     | N/A              |

