0x61 — Optical sensor settings
==============================

These settings are not really understood but they seem to be related to the optical sensor. Values can be read or written.

The parameters are:

| Bytes | Type       | Content            | Comment                  |
| ----- | ---------- | ------------------ | ------------------------ |
| 0     | byte       | Volatile sensor-related value | Read-only. Maybe the reflectiveness of the material under the sensor |
| 1     | byte       | Movement filter?   | Same as byte 36 in the profile |
| 2     | byte       | ?                  | Same as byte 37 in the profile |

Byte 0 is near zero when in the air, around 0x30 on cloth, around 0x70 on shiny plastic or glass.

