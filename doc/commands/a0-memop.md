0xA0 – Memory operation
=======================

These long report performs operations on the internal memory.

Writing operation (either from this command or 0x9x subIDs) work differently based on the location. In RAM (page 0), data replace the previous values. In flash memory (other pages), the data is AND’ed, so you need to fill the page with 0xFF using the *clear* operation before writing in order to replace.

Clear page (operation 2)
------------------------

Fill a whole page with 0xFF. It is not possible to clear only a block.

| Bytes | Type       | Content            |
| ----- | ---------- | ------------------ |
| 0     | byte       | operation = 2      |
| 1–5   |            | ?                  |
| 6     | byte       | Page to clear      |
| 7–15  |            | ?                  |


Copy memory (operation 3)
-------------------------

Copy a block of *length* bytes from *source* to *destination*.

| Bytes | Type       | Content            |
| ----- | ---------- | ------------------ |
| 0     | byte       | operation = 3      |
| 1     |            | ?                  |
| 2     | byte       | source page        |
| 3     | byte       | source offset      |
| 4–5   |            | ?                  |
| 6     | byte       | destination page   |
| 7     | byte       | destination offset |
| 8–9   |            | ?                  |
| 10–11 | int16  be  | length             |
| 12–15 |            | ?                  |


Compute page CRC (operation 4)
------------------------------

Write the CRC of *page count* pages starting at *source page*.

| Bytes | Type       | Content            |
| ----- | ---------- | ------------------ |
| 0     | byte       | operation = 4      |
| 1     |            | ?                  |
| 2     | byte       | source page        |
| 3     | byte       | page count         |
| 4–5   |            | ?                  |
| 6     | byte       | destination page   |
| 7     | byte       | destination offset |
| 8–15  |            | ?                  |

