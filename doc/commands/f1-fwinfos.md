0xF1 – Firmware informations
============================

The first parameter byte specify what info to read:
- 0x01 read firmware version. Bytes 1 and 2 in the answer contain respectively the firmware major and minor version number (in BCD).
- 0x02 read the build number. Bytes 1 and 2 in the answer is the 16 bits big-endian build number.

