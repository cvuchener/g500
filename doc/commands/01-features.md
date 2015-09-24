0x01 – Individual features
==========================

Features bits are in parameter bytes 0 and 2:

| Bit | Feature                    | Set      | Unset    |
| --- | -------------------------- | -------- | -------- |
| 1   | Special button function    | generic  | special  |
| 2   | Enhanced Key Usage         | normal   | enhanced |
| 3   | Fast Forward/Rewind        | disabled | enabled  |
| 6   | Scrolling acceleration     | disabled | enabled  |
| 7   | Buttons control resolution | generic  | special  |
| 16  | Inhibit Lock Key sound     | sound    | no sound |
| 18  | 3D engine                  | disabled | enabled  |
| 19  | LEDs control               | hardware | software |

### G5

The G5 uses this to set some buttons behavior: “Special button function” controls the wheel tilt buttons, “Buttons control resolution” controls the + and − buttons.

### G500

The G500 only supports the “Scrolling acceleration” feature.

