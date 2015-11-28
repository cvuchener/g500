0x0F – Profile queries
======================

Supported devices:
 - G500 (c068)
 - G500s (c24e)

Writing this register will load a profile. Reading gives the index of the current profile (if there is any).

The first parameter byte change the way the profile is loaded.


0xFF – Factory default
----------------------

0xFF load the factory default profile. This profile is also loaded when the profile directory page contains invalid data.

This value is read from the register when the factory default is loaded.

LGS load this profile before writing a new profile but it is not necessary.

Parameters (read and write) are:

| Bytes | Content        |
| ----- | -------------- |
| 0     | 0xFF           |
| 1–2   | Unused (0x00)  |


0x00 – Profile index
--------------------

0x00 (or 0x02) load the profile from the [profile directory](../memory.md#profile-directory-page-1) with the given index.

Using an index too high will load the last profile.

LGS use this method after writing the new profile in order to update the current profile from memory.

Parameters (read and write) are:

| Bytes | Content        |
| ----- | -------------- |
| 0     | 0x00           |
| 1     | Profile index  |
| 2     | Unused (0x00)  |


0x01 – Profile address
----------------------

0x01 (or 0x03) load the profile from the given address.

After loading a profile with this method, reading the register will gives the profile index (byte 0 is 0x00), as if the current profile was overwritten by the new profile.

Parameters (write only) are:

| Bytes | Content        |
| ----- | -------------- |
| 0     | 0x01           |
| 1     | Profile page   |
| 2     | Profile offset |

