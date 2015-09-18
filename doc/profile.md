Profile
=======

A profile must be written at the beginning of a memory page. It fits in page 0, so it can be used for temporary profiles. The remaining space in the page can be used for other purposes: string macros or driver metadata (LGS stores macro names just after the profile).

When plugging the mouse, the profile from page 2 is loaded.

There are different structures for profiles:
 - [Profile for G9](profile-g9.md) supported by:
   - G9 (c048)
 - [Profile for G500](profile-g500.md) supported by:
   - G500 (c068)
   - G500s (c24e)

Button binding
--------------

Supported devices:
 - G9 (c048)
 - G500 (c068)
 - G500s (c24e)

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
| 1–2   | int16 le   | special function   | bit-field                |

The special functions are:
 - 0x0001: pan left (horizontal wheel −1).
 - 0x0002: pan right (horizontal wheel +1).
 - 0x0004: switch to next DPI mode (default action of ‘+’ button).
 - 0x0008: switch to previous DPI mode (default action of ‘−’ button).


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

