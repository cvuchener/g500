0x51 – LEDs
===========

The format for sending or reading is the same.
The LEDs state is stored in a 16 bits little-endian integer where each half-byte represents a LED. 1 means the LED is off, 2 means it is on.
Each half-byte from lowest to highest corresponds to:
 - High
 - Mid
 - Low
 - “Running man” icon

Example reading LED state:
```
out  10 00 81 51 00 00 00
in   10 00 81 51 22 12 00
```
The two lowest LEDs and the “running man” are on. The highest LED is off.

With the G5, only the three regular LEDs can be read/set

