0x64 – USB refresh rate
=======================

The USB refresh rate is encoded in a single byte. It is the polling period in milliseconds (1000/frequency).

Accepted values are:

| Value | Refresh rate |
| ----- | ------------ |
| 0x01  | 1000 Hz      |
| 0x02  | 500 Hz       |
| 0x03  | 333 Hz       |
| 0x04  | 250 Hz       |
| 0x05  | 200 Hz       |
| 0x08  | 125 Hz       |

