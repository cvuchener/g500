Profile
=======

Structure
---------

The profile is 78 bytes long.
 - Bytes 0-3: unknown purpose usually `FF 00 00 80` but may vary.
 - Bytes 4-33: 5 6-byte-long DPI modes.
 - Byte 34: Angle correction (1: off, 2: on).
 - Byte 35: Default DPI mode (current mode when plugging the mouse or loading profile).
 - Byte 36: Some kind of movement filter (acceleration?), default is 0x10.
 - Byte 37: unknown (usually 0x10).
 - Byte 38: USB refresh rate (fraction of 1000Hz)
 - Byte 39-77: 13 3-byte-long button bindings

### DPI mode

Foreach DPI mode, the structure is:
 - Bytes 0-1: 16 bits big-endian X resolution (dpi × 17 ÷ 400)
 - Bytes 2-3: 16 bits big-endian Y resolution (dpi × 17 ÷ 400)
 - Bytes 4-5: 16 bits little-endian leds status (1: off, 2: on)

All bytes are null when the mode is disabled.

### Button binding

The first byte indicate the binding type, the next two bytes depends on the type:
 - 0x81 generic mouse button. The next two bytes are a 16 bits little-endian integer with exactly one bit set corresponding to the target button (same structure as in the HID mouse report).
 - 0x82 keyboard keys. The first byte contains a bit field indicating the modifier keys status. The second contains the regular key usage code. Again the two bytes use the same structure as in the HID keyboard report.
 - 0x83 special G500 function. The next bytes are a 16 bits little-endian bit-field indicating the function:
   * 0x0001: pan left (horizontal wheel −1).
   * 0x0002: pan right (horizontal wheel +1).
   * 0x0004: switch to next DPI mode (default action of ‘+’ button).
   * 0x0008: switch to previous DPI mode (default action of ‘−’ button).
 - 0x84 consumer control. The next bytes are a 16 bits big-endian integer containing the HID usage from the consumer control page.
 - 0x8F no binding, the button does nothing.
 - Macro has no type value but use the page number of the start of the macro instead. The next byte give the offset. The last one is unused.
