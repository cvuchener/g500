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

| Value | Description                                           | R/W | UR    | G5    | G500(s) | G700s |
| ----- | ----------------------------------------------------- | --- | ----- | ----- | ------- | ----- |
| 0x00  | Enable notifications                                  | R/W | short | short |         | short |
| 0x01  | [Individual features](commands/01-features.md)        | R/W |       | short | short   | short |
| 0x02  | Connection state                                      | R/W | short |       |         |       |
| 0x07  | ?                                                     | R/? |       |       |         | short |
| 0x0F  | [Profile related queries](commands/0f-profile.md)     | R/W |       |       | short   | short |
| 0x51  | [LEDs status](commands/51-leds.md)                    | R/W |       | short | short   | short |
| 0x57  | LEDs color                                            | R/W |       |       | short   |       |
| 0x61  | [Optical sensor settings](commands/61-sensor.md)      | R/W |       |       | short   | short |
| 0x63  | [Current resolution](commands/63-resolution.md)       | R/W |       | short | long    | long  |
| 0x64  | [USB refresh rate](commands/64-usbrate.md)            | R/W |       | short | short   | short |
| 0x78  | ?                                                     | R   |       |       | short   |       |
| 0xA0  | [Internal memory operations](commands/a0-memop.md)    | W   |       |       | long    | ?     |
| 0xA1  | [Resetting the sequence number](commands/a1-seqnum.md)| W   |       |       | short   | ?     |
| 0xA2  | [Internal memory reading](commands/a2-memread.md)     | R   |       |       | long    | long  |
| 0xB2  | Device Connection (Pairing)                           | W   | short |       |         |       |
| 0xB3  | Device Activity                                       | R   | long  |       |         |       |
| 0xB5  | Device Pairing information                            | R/W | long  |       |         |       |
| 0xD0  | ?                                                     | R/W |       | short | short   | short |
| 0xD2  | ? (write unavailable on G5)                           | R   |       | short | short   |       |
| 0xD3  | ? (error: unavailable)                                | R/? |       |       |         | long  |
| 0xD5  | ? (error: unavailable)                                | R/? |       |       |         | long  |
| 0xDA  | ? (error: unavailable)                                | R/W |       | short | short   |       |
| 0xDB  | ? (error: unavailable)                                | R   |       | long  | long    |       |
| 0xDE  | ? (error: unavailable)                                | R/W |       | short | short   |       |
| 0xF0  | ?                                                     | W   |       | short | short   | ?     |
| 0xF1  | [Firmware infos](commands/f1-fwinfos.md)              | R   |       | short | short   | ?     |

The rest of the bytes are specific to each case.


Sending data to the internal memory
-----------------------------------

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
