Profile
=======

A profile must be written at the beginning of a memory page. It fits in page 0, so it can be used for temporary profiles. The remaining space in the page can be used for other purposes: sotring macros or driver metadata (LGS stores macro names just after the profile).

When plugging the mouse, the profile from page 2 is loaded.

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

Foreach DPI mode, the structure is:

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0–1   | int16 be   | X resolution       | dpi × 17 ÷ 400           |
| 2–3   | int16 be   | Y resolution       | dpi × 17 ÷ 400           |
| 4–5   | int16 le   | leds status        | each half-byte is a led: 1: off, 2: on  |

All bytes are null when the mode is disabled.

For the leds status, each half-byte from lowest to highest corresponds to:
 - High
 - Mid
 - Low
 - “Running man” icon

### Button binding

The first byte indicate the binding type, the next two bytes depends on the type:
 - 0x81 generic mouse button.

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | binding type       | 0x81                     |
	| 1–2   | int16 le   | mouse button       | bit-field                |

   The mouse button uses the same structure as in the HID mouse report.
 - 0x82 keyboard keys.

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | binding type       | 0x82                     |
	| 1     | byte       | modifier           | bit-field, usage from 0xE0 to 0xE7 |
	| 2     | byte       | key usage          |                          |

   The modifier bit-field and the key usage use the same structure as in the HID keyboard report.
 - 0x83 special G500 function.

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | binding type       | 0x83                     |
	| 1–2   | int16 le   | special function   | bit-field                |

   The special functions are:
    * 0x0001: pan left (horizontal wheel −1).
    * 0x0002: pan right (horizontal wheel +1).
    * 0x0004: switch to next DPI mode (default action of ‘+’ button).
    * 0x0008: switch to previous DPI mode (default action of ‘−’ button).
 - 0x84 consumer control. The next bytes are a 16 bits big-endian integer containing the HID usage from the consumer control page.

	| Bytes | Type       | Content                | Comment                  |
	| ----- | ---------- | ---------------------- | ------------------------ |
	| 0     | byte       | binding type           | 0x84                     |
	| 1–2   | int16 be   | consumer control usage |                          |

 - 0x8F no binding, the button does nothing.

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | binding type       | 0x8F                     |
	| 1–2   |            |                    | zeros                    |

 - Macro has no type value but use the page number of the start of the macro instead.

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | macro page         |                          |
	| 1     | byte       | macro offset       |                          |
	| 2     |            |                    | 0x00                     |

