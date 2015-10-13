0x0D — Battery mileage
======================

Values are little-endian.

| Bits  | Description         |
| ----- | ------------------- |
| 0–6   | Level (1 unit = 1%) |
| 8–19  | Maximum             |
| 20–21 | Unit                |
| 22–23 | State               |

| Unit | Description |
| ---- | ----------- |
| 0    | Seconds     |
| 1    | Minutes     |
| 2    | Hours       |
| 3    | Days        |

| State | Description       |
| ----- | ----------------- |
| 0     | Not charging      |
| 1     | Charging          |
| 2     | Charging complete |
| 3     | Charging error    |
