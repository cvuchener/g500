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
 - "Running man" icon

Example reading LED state:
```
Send 10 00 81 51 00 00 00
Recv 10 00 81 51 22 12 00
```
The two lowest LEDs and the "running man" are on. The highest LED is off.


#### Resolution (0x63)

The resolution is encoded in two 16 bits integer, one for the X axis and one for the Y axis. The value are not in dpi, you must multiply it by 400/17 to get the dpi value.

The data size for these queries is 6 bytes: 4 bytes for the two resolution value in little-endian and 2 bytes for unknown purposes. Writing those 2 bytes does not seem to change anything. When reading they usually are 80 01, but they can also be FF FF for an invalid profile. The rest of the bytes are uninitialized (they have the same value as in the last long report from the device).

Example:
```
Send 10 00 83 63 00 00 00
Recv 11 00 83 63 22 00 22 00 80 01 00 00 00 00 00 00 00 00 00 00
```
The current resolution is 400dpi (0x0022).
```
Send 11 00 82 63 F3 00 F3 00 00 00 00 00 00 00 00 00 00 00 00 00
Recv 10 00 82 63 00 00 00
```
Set the current resolution to 5700dpi (0x00F3).
```
Send 10 00 83 63 00 00 00
Recv 11 00 83 63 F3 00 F3 00 80 01 00 00 00 00 00 00 00 00 00 00
```
The current resolution is now 5700dpi.

Higher resolution than the maximum (5700dpi = 0x00F3) will not generate any error but they will not be faster.


#### USB refresh rate (0x64)

#### Memory operation (0xA0)

#### Sequence number (0xA1)

#### Reading memory (0xA2)


Writing data in the memory
--------------------------

### Message types

### Header

### Acknowledgement messages

Error messages
--------------

