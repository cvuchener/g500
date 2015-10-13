HID++ 1.0
=========

Short and long reports are use the same way. The long report is used when the data does not fit in the short one. Every HID++ 1.0 report use the same header:

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0     | byte       | HID report ID      | 0x10 for short report, 0x11 for long report |
| 1     | byte       | Device index       | -1 (0xff) for Unifying receivers, 0 for wired devices, 1–6 for Unifying devices |
| 2     | byte       | Sub ID             |                          |
| 3–…   |            |                    | Depends on the *Sub ID*  |


Known Sub IDs are:
  - 0x40–…: Notifications
  - 0x80–0x8F: Register access
  - 0x90–0x93: Writing data to memory


Register access
---------------

A register size may vary, if it is too big to fit in a short report, the long report is used. Every register command is answered with a input report using the same Sub ID or an error message.

| Sub ID | Description          | Output | Input |
| ------ | -------------------- | ------ | ----- |
| 0x80   | Write short register | short  | short |
| 0x81   | Read short register  | short  | short |
| 0x82   | Write long register  | long   | short |
| 0x83   | Read short register  | short  | long  |
| 0x8F   | Error message        | N/A    | short |

### Accessing registers

The format for report with Sub IDs 0x80 to 0x83 is:

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0     | byte       | HID report ID      | 0x10 for short report, 0x11 for long report |
| 1     | byte       | Device index       |                          |
| 2     | byte       | Sub ID             | 0x80–0x83                |
| 3     | byte       | Register index     | See below                |
| 4–…   |            |                    | Register data            |

#### 0x00–0x0F: General registers

| Register | Description                                           | R/W | UR    | G5    | G500(s) | G700s |
| -------- | ----------------------------------------------------- | --- | ----- | ----- | ------- | ----- |
| 0x00     | Enable notifications                                  | R/W | short | short |         | short |
| 0x01     | [Individual features](commands/01-features.md)        | R/W |       | short | short   | short |
| 0x02     | Connection state                                      | R/W | short |       |         |       |
| 0x07     | [Battery status](commands/07-battery-status.md)       | R/? |       |       |         | short |
| 0x0F     | [Profile related queries](commands/0f-profile.md)     | R/W |       |       | short   | short |

#### 0x20–0x2F: Pairing information registers (UR)

#### 0x30–0x3F: Extended pairing information registers (UR)

#### 0x40–0x4F: Device name registers (UR)

#### 0x50–0x5F: LED registers

| Register | Description                                           | R/W | G5    | G500(s) | G700s |
| -------- | ----------------------------------------------------- | --- | ----- | ------- | ----- |
| 0x51     | [LEDs status](commands/51-leds.md)                    | R/W | short | short   | short |
| 0x57     | LEDs color                                            | R/W |       | short   |       |

#### 0x60–0x6F: Mouse sensor registers

| Register | Description                                           | R/W | G5    | G500(s) | G700s |
| -------- | ----------------------------------------------------- | --- | ----- | ------- | ----- |
| 0x61     | [Optical sensor settings](commands/61-sensor.md)      | R/W |       | short   | short |
| 0x63     | [Current resolution](commands/63-resolution.md)       | R/W | short | long    | long  |
| 0x64     | [USB refresh rate](commands/64-usbrate.md)            | R/W | short | short   | short |

#### 0x70–0x7F: ?

| Register | Description                                           | R/W | G500(s) |
| -------- | ----------------------------------------------------- | --- | ------- |
| 0x78     | ?                                                     | R   | short   |

#### 0xA0–0xAF: Memory registers

| Register | Description                                           | R/W | G500(s) | G700s |
| -------- | ----------------------------------------------------- | --- | ------- | ----- |
| 0xA0     | [Internal memory operations](commands/a0-memop.md)    | W   | long    | ?     |
| 0xA1     | [Resetting the sequence number](commands/a1-seqnum.md)| W   | short   | ?     |
| 0xA2     | [Internal memory reading](commands/a2-memread.md)     | R   | long    | long  |

#### 0xB0–0xBF: Unifying connections

| Register | Description                                           | R/W | UR    |
| -------- | ----------------------------------------------------- | --- | ----- |
| 0xB2     | Device Connection (Pairing)                           | W   | short |
| 0xB3     | Device Activity                                       | R   | long  |
| 0xB5     | Device Pairing information                            | R/W | long  |

#### 0xD0–0xDF: ?

| Register | Description                                           | R/W | G5    | G500(s) | G700s |
| -------- | ----------------------------------------------------- | --- | ----- | ------- | ----- |
| 0xD0     | ?                                                     | R/W | short | short   | short |
| 0xD2     | ? (write unavailable on G5)                           | R   | short | short   |       |
| 0xD3     | ? (error: unavailable)                                | R/? |       |         | long  |
| 0xD5     | ? (error: unavailable)                                | R/? |       |         | long  |
| 0xDA     | ? (error: unavailable)                                | R/W | short | short   |       |
| 0xDB     | ? (error: unavailable)                                | R   | long  | long    |       |
| 0xDE     | ? (error: unavailable)                                | R/W | short | short   |       |

#### 0xF0–0xFF: Firmware registers?

| Register | Description                                           | R/W | G5    | G500(s) | G700s |
| -------- | ----------------------------------------------------- | --- | ----- | ------- | ----- |
| 0xF0     | ?                                                     | W   | short | short   | ?     |
| 0xF1     | [Firmware infos](commands/f1-fwinfos.md)              | R   | short | short   | ?     |


### Error messages

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0     | byte       | HID report ID      | Short report: 0x10       |
| 1     | byte       | Device index       |                          |
| 2     | byte       | Sub ID             | 0x8F                     |
| 3     | byte       | Original Sub ID    | From the output report generating the error |
| 4     | byte       | Register index     | From the output report generating the error |
| 5     | byte       | Error code         | See table below          |
| 6     |            |                    | *unused*                 |

Error codes are:

| Value | Name                    | Description                           |
| ----- | ----------------------- | ------------------------------------- |
| 0x00  | ERR_SUCCESS             |                                       |
| 0x01  | ERR_INVALID_SUBID       | Invalid report type or message length |
| 0x02  | ERR_INVALID_ADDRESS     | Invalid query type                    |
| 0x03  | ERR_INVALID_VALUE       |                                       |
| 0x04  | ERR_CONNECT_FAIL        | Unifying Receiver error               |
| 0x05  | ERR_TOO_MANY_DEVICES    | Unifying Receiver error               |
| 0x06  | ERR_ALREADY_EXISTS      | Unifying Receiver error               |
| 0x07  | ERR_BUSY                | Unifying Receiver error               |
| 0x08  | ERR_UNKNOWN_DEVICE      | Unifying Receiver error               |
| 0x09  | ERR_RESOURCE_ERROR      | Unifying Receiver error               |
| 0x0A  | ERR_REQUEST_UNAVAILABLE |                                       |
| 0x0B  | ERR_INVALID_PARAM_VALUE |                                       |
| 0x0C  | ERR_WRONG_PIN_CODE      |                                       |

For invalid parameters some devices use ERR_INVALID_VALUE instead of ERR_INVALID_PARAM_VALUE.


Sending data to the internal memory
-----------------------------------

Data is sent in blocks to the internal memory. Values are replaced in the temporary memory (page 0) but is AND’ed in the permanent memory.

Each block is transmitted in several messages, the first message contains a 9-byte-long header describing the block being sent. All reports are long, thus each report contains 16 bytes of data, except the first which contains only 7 bytes because of the header.

The structure of each message is:

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0     | byte       | Report ID          | Long report: 0x11        |
| 1     | byte       | Device index       |                          |
| 2     | byte       | Sub ID             | 0x90–0x93                |
| 3     | byte       | Sequence number    |                          |
| 4–19  | byte array | 16 bytes of data   | Payload or header with smaller payload |

### Sub IDs

Sub IDs identify if the report is the first for a block or if acknowledgment message (input report with Sub ID = 0x50) are needed.

|                         | Begin sending | Continue sending |
| ----------------------- | ------------- | ---------------- |
| Without acknowledgement | 0x90          | 0x91             |
| With acknowledgement    | 0x92          | 0x93             |


### Sequence number

Each message contains a sequence number that must be incremented each time. The next sequence number to send can be reset by writing [register 0xA1](commands/a1-seqnum.md).

When using acknowledgement message, the sequence number is repeated in the input report.

### Acknowledgement messages

The structure of the acknowledgement messages is:

| Bytes | Type       | Content                | Comment                  |
| ----- | ---------- | ---------------------- | ------------------------ |
| 0     | byte       | Report ID              | Short report: 0x10       |
| 1     | byte       | Device index           |                          |
| 2     | byte       | Sub ID                 | 0x50                     |
| 3     | byte       | Acknowledgement status | 0x01 if there is no error, otherwise an error code |
| 4     | byte       | Sequence number        | if there was no error    |
| 5–6   |            |                        | *unused*                 |


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

