0xA0 – Memory operation
=======================

These long report performs operations on the internal memory. Following bytes in the report are:

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0     | byte       | operation          |                          |
| 1     |            | ?                  |                          |
| 2     | byte       | source offset      | for operation 4          |
| 3     | byte       | source length      | for operation 4          |
| 4–5   |            | ?                  |                          |
| 6     | byte       | destination page   |                          |
| 7     | byte       | destination offset | unused for operation 2   |
| 8–9   |            | ?                  |                          |
| 10–11 | int16  be  | length             | for operation 3          |
| 12–15 |            | ?                  |                          |

The known operation are:
 - 2: Fill the *destination page* with 0xFF (does not use *offset* or *length*). This this useful before AND’ing data. This has no effect on page 0 (temporary memory).
 - 3: Copy or AND *length* bytes of data in page 0 to the *destination page* at *destination offset*
 - 4: Copy or AND *source length* bytes from *source offset* from an unknown static source of data at *destination offset* in *destination offset*. I have seen the report in Vladyslav Shtabovenko’s g500-control.c, SetPoint then read what was written but I don’t know why.

Writing data works differently based on the page. In page 0, data replace the previous values. In other pages, the data is AND’ed, so you need to fill the page with 0xFF before writing in order to replace.

