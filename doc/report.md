Report
======

Logitech uses two kind of report for configuring the device. Despite having different HID usage in the HID descriptor they are used for the same purpose. They are not used for transmitting HID usages but transmitting byte array like some network datagram protocol. The longer report is used when the data does not fit in the shorter one.

The short report has id 0x10 and contains 6 bytes. The long report has id 0x11 and contains 19 bytes.

To communicate with the device you must send an output report to the device and the device will usually answer with an input report. The input report may have a different report id than the output one.

The first byte in the report is always zero. The second byte indicates the type of the message. The rest depends on the message type.

Querying the device
-------------------

### Message types

The message type depends on whether you are sending or receiving data and the its length.
 - 0x80 for sending short data to the device.
 - 0x81 for receiving short data from the device.
 - 0x82 for sending long data to the device.
 - 0x83 for receiving long data from the device.

The report that does not contain the data (the output one when receiving or the input one when sending) is a short report. The answer will contain the same message type except if there was an error (message type 0x8F).

### Message content

The next byte in the report indicate what to query, for the G500 they are:
 - 0x0F for profile related queries (short).
 - 0x51 for LEDs status (short) (same as for the G5).
 - 0x63 for the resolution for the current mode (long) (same as for the G5 but the G5 use a short format).
 - 0x64 for the USB refresh rate (short).
 - 0xA0 for performing operations on the memory (long).
 - 0xA1 is used for resetting the sequence number used for sending data to the memory (short).
 - 0xA2 for reading the content of the memory (long).

The rest of the bytes are specific to each case.

#### Profile queries (0x0F)

When sending, this message changes the current profile.
There seems to be 5 possible value for the first byte.
 - 0xFF is used by LGS before writing a new profile. It seems to set a default profile not from the memory.
 - 0x00 is used by LGS after writing the new profile. It sets the mouse to use the default profile at page 0x02.
 - 0x01 sets the current profile to one whose page is in the next byte. (10 00 0F 01 03 00 set the current profile to the one in page 0x03).
 - 0x02 looks similar to 0x00.
 - 0x03 looks similar to 0x01.

Other values will get an error message.

When reading, the first byte will be 0x00 if a profile from memory is used or 0xFF if no profile is used.


#### LEDs (0x51)

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


#### Resolution (0x63)

The resolution is encoded in two 16 bits integer, one for the X axis and one for the Y axis. The value are not in dpi, you must multiply it by 400/17 to get the dpi value.

The data size for these queries is 6 bytes: 4 bytes for the two resolution value in little-endian and 2 bytes for unknown purposes. Writing those 2 bytes does not seem to change anything. When reading they usually are 80 01, but they can also be FF FF for an invalid profile. The rest of the bytes are uninitialized (they have the same value as in the last long report from the device).

Example:
```
out  10 00 83 63 00 00 00
in   11 00 83 63 22 00 22 00 80 01 00 00 00 00 00 00 00 00 00 00
```
The current resolution is 400dpi (0x0022).
```
out  11 00 82 63 F3 00 F3 00 00 00 00 00 00 00 00 00 00 00 00 00
in   10 00 82 63 00 00 00
```
Set the current resolution to 5700dpi (0x00F3).
```
out  10 00 83 63 00 00 00
in   11 00 83 63 F3 00 F3 00 80 01 00 00 00 00 00 00 00 00 00 00
```
The current resolution is now 5700dpi.

Higher resolution than the maximum (5700dpi = 0x00F3) will not generate any error but they will not be faster.


#### USB refresh rate (0x64)

The USB refresh rate is encoded in a single byte. The value is the divisor of 1000 Hz:
 - 0x01 = 1000 Hz
 - 0x02 = 500 Hz
 - 0x03 = 333 Hz
 - 0x04 = 250 Hz
 - 0x05 = 200 Hz
 - 0x08 = 125 Hz


#### Memory operation (0xA0)

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


#### Sequence number (0xA1)

The only known report with this value is: `10 00 80 A1 01 00 00`. It is used for resetting the sequence number used by the memory writing reports (message type 0x90-0x93).


#### Reading memory (0xA2)

This message is used to retrieve data (exactly 16 bytes) from the internal memory. The query parameters are the page and offset of the data you want to read. The answer last 16 bytes are the data.

Here is an example of LGS reading the data just after the profile (offset 0x27 means from byte 0x4E) in page 2. The data is a string (`4C 47 53 30 32` = "LGS02") that LGS stores there for itself.
```
out  10 00 83 A2 02 27 00
in   11 00 83 A2 4C 47 53 30 32 00 00 00 00 00 00 00 00 00 00 00
```


Sending data to the internal memory
--------------------------

Data is sent in blocks to the internal memory. Values are replaced in the temporary memory (page 0) but is AND’ed in the permanent memory.

Each block is transmitted in several messages, the first message contains a header describing the block being sent. The header is 9 byte long.

The structure of each message is:

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0     | byte       | long report ID     | 0x11                     |
| 1     | byte       | zero               | 0x00                     |
| 2     | byte       | message type       | 0x90–0x93                |
| 3     | byte       | a sequence number  |                          |
| 4–19  | byte array | 16 bytes of data   | The 9 first are the header in the first message, so there is actually only 7 bytes of payload in the first message. |

### Message types

There are four message types. Depending if it is the first message (with a header) or if an acknowledgement message is required.

|                         | Begin sending | Continue sending |
| ----------------------- | ------------- | ---------------- |
| Without acknowledgement | 0x90          | 0x91             |
| With acknowledgement    | 0x92          | 0x93             |


### Sequence number & Acknowledgement messages

Each message contains a sequence number that must be incremented each time. The next sequence number to send can be reset with a special query (0xA1).

When using the message types requiring an acknowledgement, the sequence number is repeated in the acknowledgement message.

The structure of the acknowledgement messages is:

| Bytes | Type       | Content                | Comment                  |
| ----- | ---------- | ---------------------- | ------------------------ |
| 0     | byte       | short report ID        | 0x10                     |
| 1     | byte       | zero                   | 0x00                     |
| 2     | byte       | message type           | 0x50                     |
| 3     | byte       | acknowledgement status | 0x01 if there is no error, otherwise an error code |
| 4     | byte       | the sequence number    | if there was no error
| 5–6   |            | two null bytes         |                          |


### Header

The structure of the header sent in the first message is:

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0     | byte       | ?                  | 0x01                     |
| 1     | byte       | destination page   |                          |
| 2     | byte       | destination offset |                          |
| 3–4   | byte       | ?                  |                          |
| 5–6   | int16 be   | block length       |                          |
| 7–8   | byte       | ?                  |                          |


Error messages
--------------

The mouse will sometimes answer with error messages. The error message use a short report with message type 0x8F. The next bytes contains the message type and query type from the message that generated the error and an error code (8 bits or 16 bts little-endian?).

### Examples

Sending an invalid resolution setting, the error message repeat the message type (`80 63`) and gives error code 1.
```
out  10 00 80 63 00 00 00
in   10 00 8F 80 63 01 00
```

Sending an invalid request with 0xA1 gives another error code 11.
```
out  10 00 80 A1 00 00 00
in   10 00 8F 80 A1 0B 00
```

