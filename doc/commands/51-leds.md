0x51 – LEDs
===========

Each half-byte is a LED (low half-byte comes first).

| Value | Description      |
| ----- | ---------------- |
| 0x0	| No change/No LED |
| 0x1   | Off              |
| 0x2   | On               |
| 0x3   | Blink            |
| 0x4   | Heartbeat        |
| 0x5   | Slow on          |
| 0x6   | Slow off         |

G5
--

Only “Off” and “On” are supported. Other values don’t change the LED state.

| Index | LED    |
| ----- | ------ |
| 0     | Low    |
| 1     | Middle |
| 2     | High   |

The “Running man” logo is always on.


G500
----

Only “Off” and “On” are supported. “Blink” and “Slow on” set the LED off, “Slow off” set the LED on, “Heartbeat” don’t change the LED state.

| Index | LED                |
| ----- | ------------------ |
| 0     | “Running man” logo |
| 1     | Low                |
| 2     | Middle             |
| 3     | High               |

