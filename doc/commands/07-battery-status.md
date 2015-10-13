0x07 – Battery status
=====================

| Bytes | Type       | Content            | Comment                       |
| ----- | ---------- | ------------------ | ----------------------------- |
| 0     | byte       | Level              | See values in table below     |
| 1     | byte       | State              | See values in table below     |
| 2     | byte       | Low threshold      | Between 0 and 6, 1 unit is 5% |


Level values
------------

| Value | Description       |
| ----- | ----------------- |
| 0x00  | Unknown           |
| 0x01  | Critical          |
| 0x02  | Critical (legacy) |
| 0x03  | Low               |
| 0x04  | Low (legacy)      |
| 0x05  | Good              |
| 0x06  | Good (legacy)     |
| 0x07  | Full (legacy)     |


State values
------------

| Value | Description       |
| ----- | ----------------- |
| 0x00  | Not charging      |
| 0x20  | Unknown           |
| 0x21  | Charging          |
| 0x22  | Charging complete |
| 0x23  | Charging error    |
| 0x24  | Charging fast     |
| 0x25  | Charging slow     |
| 0x26  | Topping charge    |

