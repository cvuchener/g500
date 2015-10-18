0x61 — Optical sensor settings
==============================

These settings are not really understood but they seem to be related to the optical sensor. Values can be read or written.

The parameters are:

| Bytes | Type       | Content                       | Comment                  |
| ----- | ---------- | ----------------------------- | ------------------------ |
| 0     | byte       | Volatile sensor-related value | Read-only. Maybe the reflectiveness of the material under the sensor |
| 1     | byte       | Lift threshold?               | 0 = no change. default = 0x10 |
| 2     | byte       | ?                             | Same as byte 37 in the profile |

Byte 0 is near zero when in the air, around 0x30 on cloth, around 0x70 on shiny plastic or glass.

The G500 only uses the first 5 bits of byte 1 (lift threshold?): values higher than 0x1F are accepted but behave the same way as those from the 0x01–0x1F range. It may be linked to the `Lift_Detection_Thr` register from the S9500 sensor.
