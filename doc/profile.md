Profile
=======

A profile must be written at the beginning of a memory page. It fits in page 0, so it can be used for temporary profiles. The remaining space in the page can be used for other purposes: string macros or driver metadata (LGS stores macro names just after the profile).

When plugging the mouse, the profile from page 2 is loaded.

There are different structures for profiles:
 - [Profile for G9](profile/g9.md) supported by:
   - G9 (c048)
 - [Profile for G500](profile/g500.md) supported by:
   - G500 (c068)
   - G500s (c24e)
 - [Profile for G700s](profile/g700s.md) supported by:
   - G700s (c70c)

Button binding
--------------

Supported devices:
 - G9 (c048)
 - G500 (c068)
 - G500s (c24e)
 - G700s (c70c)

Each button is 3 byte long.  The first byte indicate the binding type, the next two bytes depends on the type:

### 0x81 – Generic mouse button

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0     | byte       | binding type       | 0x81                     |
| 1–2   | int16 le   | mouse button       | bit-field                |

The mouse button uses the same structure as in the HID mouse report.


### 0x82 – Keyboard keys

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0     | byte       | binding type       | 0x82                     |
| 1     | byte       | modifier           | bit-field, usage from 0xE0 to 0xE7 |
| 2     | byte       | key usage          |                          |

The modifier bit-field and the key usage use the same structure as in the HID keyboard report.

### 0x83 – Special G500 function

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0     | byte       | binding type       | 0x83                     |
| 1–2   | int16 le   | special function   |                          |

The special functions are:
 - 0x0001: pan left (horizontal wheel −1) (G500(s), G700s).
 - 0x0002: pan right (horizontal wheel +1) (G500(s), G700s).
 - 0x0003: battery level (G700s)
 - 0x0004: next DPI mode (G500(s), G700s).
 - 0x0005: cycle DPI mode up (G700s).
 - 0x0008: previous DPI mode (G500(s), G700s).
 - 0x0009: cycle DPI mode down (G700s)
 - 0x0010: next profile (G500, G700s)
 - 0x0011: cycle profile (G700s)
 - 0x0020: previous profile (G500, G700s)
 - 0x0040: switch to profile 1 (G700s)
 - 0x0080: battery level (G700s)
 - 0x0100: battery level (G700s)
 - 0x0105: same as 0x0005 used in default profile on page 25 (G700s)
 
### 0x84 – Consumer control

The next bytes are a 16 bits big-endian integer containing the HID usage from the consumer control page.

| Bytes | Type       | Content                | Comment                  |
| ----- | ---------- | ---------------------- | ------------------------ |
| 0     | byte       | binding type           | 0x84                     |
| 1–2   | int16 be   | consumer control usage |                          |


### 0x8F – Disabled

No binding, the button does nothing.

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0     | byte       | binding type       | 0x8F                     |
| 1–2   |            |                    | zeros                    |


### Macro

Macro has no type value but use the page number of the start of the macro instead.

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0     | byte       | macro page         |                          |
| 1     | byte       | macro offset       |                          |
| 2     |            |                    | 0x00                     |

