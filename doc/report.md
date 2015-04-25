Report
======

Logitech uses two kind of report for configuring the device. Despite having different HID usage in the HID descriptor they are used for the same purpose. They are not used for transmitting HID usages but transmitting byte array like some network datagram protocol. The longer report is used when the data does not fit in the shorter one.

The short report has id 0x10 and contains 6 bytes. The long report has id 0x11 and contains 19 bytes.

To communicate with the device you must send an output report to the device and the device will usually answer with an input report. The input report may have a different report id than the output one.

The first byte in the report is the device index on unifying receiver, it is always zero on wired mice. The second byte indicates the type of the message. The rest depends on the message type.

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0     | byte       | HID report ID      | 0x10 for short report, 0x11 for long report |
| 1     | byte       | Device index       | Used by Unifying receivers, 0 for wired devices |
| 2     | byte       | Message type       |                          |
| 3–…   |            | Parameters         | Depends on the message type |

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

The next byte in the report indicate what to query. The length of the data depends on the device.

| Value | Description                                      | R/W | G5    | G500(s) |
| ----- | ------------------------------------------------ | --- | ----- | ------- |
| 0x00  | Notifications on the unifuing receiver           | R/W | short |         |
| 0x01  | G5 special button behaviour (unknown for G500)   | R/W | short | short   |
| 0x0F  | Profile related queries                          | R/W |       | short   |
| 0x51  | LEDs status                                      | R/W | short | short   |
| 0x57  | ?                                                | R/W |       | short   |
| 0x61  | Optical sensor settings                          | R/W |       | short   |
| 0x63  | Current resolution                               | R/W | short | long    |
| 0x64  | USB refresh rate                                 | R/W | short | short   |
| 0x78  | ?                                                | R   |       | short   |
| 0xA0  | Internal memory operations                       | W   |       | long    |
| 0xA1  | Resetting the sequence number                    | W   |       | short   |
| 0xA2  | Internal memory reading                          | R   |       | long    |
| 0xD0  | ?                                                | R/W | short | short   |
| 0xD2  | ? (write unavailable on G5)                      | R   | short | short   |
| 0xDA  | ? (error: unavailable)                           | R/W | short | short   |
| 0xDB  | ? (error: unavailable)                           | R   | long  | long    |
| 0xDE  | ? (error: unavailable)                           | R/W | short | short   |
| 0xF0  | ?                                                | W   | short | short   |
| 0xF1  | ? (first byte must be 1 or 2)                    | R   | short | short   |

The rest of the bytes are specific to each case.


#### Special button behaviour (0x01)

 - **G5**: The data is a bit-field for enabling special buttons. Default value is 0x82.

| Bit | Buttons            | Behaviour                                  |
| --- | ------------------ | ------------------------------------------ |
| 1   | Wheel tilt buttons | 0 = generic buttons, 1 = horizontal wheel  |
| 4   | +/− buttons        | 0 = generic buttons, 1 = change resolution |

 - **G500**: unknown purposes. Default value is 0x00.

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

With the G5, only the three regular LEDs can be read/set


#### Resolution (0x63)

##### G500

The resolution is encoded in two 16 bits integer, one for the X axis and one for the Y axis. The value are not in dpi, for the G500, 17 units correspond to 400 dpi; for the G500s, 1 unit corresponds to 50 dpi.

The data size for these queries is 6 bytes: 4 bytes for the two resolution value in little-endian and 2 bytes for unknown purposes. Writing those 2 bytes does not seem to change anything. When reading they usually are 80 01, but they can also be FF FF for an invalid profile. The rest of the bytes are uninitialized (they have the same value as in the last long report from the device).

Example on the G500:
```
out  10 00 83 63 00 00 00
in   11 00 83 63 22 00 22 00 80 01 00 00 00 00 00 00 00 00 00 00
```
The current resolution is 400 dpi (0x0022).
```
out  11 00 82 63 F3 00 F3 00 00 00 00 00 00 00 00 00 00 00 00 00
in   10 00 82 63 00 00 00
```
Set the current resolution to 5700 dpi (0x00F3).
```
out  10 00 83 63 00 00 00
in   11 00 83 63 F3 00 F3 00 80 01 00 00 00 00 00 00 00 00 00 00
```
The current resolution is now 5700 dpi.

Higher resolution than the maximum (5700 dpi = 0x00F3 for the G500, 8200 dpi = 0x00A4 for the G500s) will not generate any error but they will not be faster.


##### G5

The resolution is a single byte.

| Value | Resolution |
| ----- | ---------- |
| 0x80  | 400 dpi    |
| 0x81  | 800 dpi    |
| 0x82  | 1600 dpi   |
| 0x83  | 2000 dpi   |

According g_hack.c, the G9 has resolution settings up to 0x87.

#### USB refresh rate (0x64)

The USB refresh rate is encoded in a single byte. The value is the divisor of 1000 Hz:

| Value | Refresh rate |
| ----- | ------------ |
| 0x01  | 1000 Hz      |
| 0x02  | 500 Hz       |
| 0x03  | 333 Hz       |
| 0x04  | 250 Hz       |
| 0x05  | 200 Hz       |
| 0x08  | 125 Hz       |


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


#### Optical sensor settings (0x61)

These settings are not really understood but they seem to be related to the optical sensor. Values can be read or written.

The parameters are:

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0     | byte       | Volatile sensor-related value | Read-only. Maybe the reflectiveness of the material under the sensor |
| 1     | byte       | Movement filter?   | Same as byte 36 in the profile |
| 2     | byte       | ?                  | Same as byte 37 in the profile |

Byte 0 is near zero when in the air, around 0x30 on cloth, around 0x70 on shiny plastic or glass.


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

### Error code

| Value | Possible meaning           |
| ----- | -------------------------- |
| 0x01  | Invalid report type or message length |
| 0x02  | Invalid query type         |
| 0x0A  | Request unavailable (according to hid-logitech-hidpp linux driver) |
| 0x0B  | Invalid parameters         |
