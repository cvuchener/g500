0xA2 – Reading memory
=====================

This message is used to retrieve data (exactly 16 bytes) from the internal memory. The query parameters are the page and offset of the data you want to read. The answer last 16 bytes are the data.

Here is an example of LGS reading the data just after the profile (offset 0x27 means from byte 0x4E) in page 2. The data is a string (`4C 47 53 30 32` = "LGS02") that LGS stores there for itself.
```
out  10 00 83 A2 02 27 00
in   11 00 83 A2 4C 47 53 30 32 00 00 00 00 00 00 00 00 00 00 00
```
