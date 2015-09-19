0x0F – Profile queries
======================

Supported devices:
 - G500 (c068)
 - G500s (c24e)

When sending, this message changes the current profile.
There seems to be 5 possible value for the first byte.
 - 0xFF is used by LGS before writing a new profile. It seems to set a default profile not from the memory.
 - 0x00 is used by LGS after writing the new profile. It sets the mouse to use the default profile at page 0x02.
 - 0x01 sets the current profile to one whose page is in the next byte. (10 00 0F 01 03 00 set the current profile to the one in page 0x03).
 - 0x02 looks similar to 0x00.
 - 0x03 looks similar to 0x01.

Other values will get an error message.

When reading, the first byte will be 0x00 if a profile from memory is used or 0xFF if no profile is used.

