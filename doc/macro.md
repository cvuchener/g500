Macros
======

Macros are a sequence of variable length instructions. As offset counts 16 bits integer, start of macros and destinations of jumps must be aligned on even addresses.

Instructions
------------

### Single byte instructions

 - **No operation (0x00)**
	Do nothing. This instruction is useful for padding if you need to align the next instruction.
 - **Wait for button release (0x01)**
 - **Repeat the macro from beginning until release (0x02)**
 - **Repeat the macro from beginning unconditionally (0x03)**
	Or I did not find how to stop it.
 - **End of macro (0xFF)**


### Two bytes instructions

 - **Keyboard key press (0x20)**

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | Opcode             | 0x20                     |
	| 1     | byte       | Keyboard HID usage |                          |

 - **Keyboard key release (0x21)**

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | Opcode             | 0x21                     |
	| 1     | byte       | Keyboard HID usage |                          |

 - **Modifier key press (0x22)**

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | Opcode             | 0x22                     |
	| 1     | byte       | Keyboard HID usage | Bit-field, usages 0xE0–0xE7 |

 - **Modifier key release (0x23)**

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | Opcode             | 0x23                     |
	| 1     | byte       | Keyboard HID usage | Bit-field, usages 0xE0–0xE7 |

 - **Mouse wheel (0x24)**

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | Opcode             | 0x24                     |
	| 1     | signed byte| Mouse wheel value  | 0x01 or 0xFF             |


### Three bytes instructions

 - **Mouse button press (0x40)**

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | Opcode             | 0x40                     |
	| 1–2   | int16 le   | Mouse button       | bit-field                |

 - **Mouse button release (0x41)**

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | Opcode             | 0x41                     |
	| 1–2   | int16 le   | Mouse button       | bit-field                |

 - **Set the current consumer control usage (0x42)**

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | Opcode             | 0x42                     |
	| 1–2   | int16 be   | Consumer control HID usage | 0x0000 for release |

 - **Delay (0x43)**

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | Opcode             | 0x43                     |
	| 1–2   | int16 be   | Delay              | in milliseconds          |

 - **Unconditional jump (0x44)**

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | Opcode             | 0x44                     |
	| 1     | byte       | Destination page   |                          |
	| 2     | byte       | Destination offset |                          |

	As LGS write a 16 bits value at the end, it uses this instruction to step over it if the macro spans multiple pages.

 - **Jump if pressed (0x45)**

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | Opcode             | 0x45                     |
	| 1     | byte       | Destination page   |                          |
	| 2     | byte       | Destination offset |                          |

### Five bytes instructions

 - **Move mouse pointer (0x60)**

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | Opcode             | 0x60                     |
	| 1–2   | int16 be   | Relative X axis    |                          |
	| 3–4   | int16 be   | Relative Y axis    |                          |

 - **Jump if released with timeout (0x61)**
	Jump to destination if the button is released before timeout, continue to the next instruction otherwise.

	| Bytes | Type       | Content            | Comment                  |
	| ----- | ---------- | ------------------ | ------------------------ |
	| 0     | byte       | Opcode             | 0x60                     |
	| 1–2   | int16 be   | Delay (timeout)    | in milliseconds          |
	| 3     | byte       | Destination page   |                          |
	| 4     | byte       | Destination offset |                          |

Examples
--------

These example are built-in examples from page 18 of the G500.

 - 12:27 Scroll the wheel while holding left control.

```
22 01	Press left control
24 01   Scroll the wheel up (+1)
a8      Delay ? (about 200ms)
02      Repeat from beginning if the button is still pressed
23 01   Release left control
ff      End of macro
```

 - 12:31 Press “mute” key

```
42 00 e2   Press “mute” (HID usage: 0xE2)
01         Wait for button release
42 00 00   Release the “mute” key
ff         End of macro
```

 - 12:54 Execute “notepad” from MS Windows execute dialog

```
22 08   Press left meta (“windows key”)
20 15   Press “R”
23 08   Release left meta
21 15   Release “R”
d9      Delay ? (about 800ms)
20 11   Press “N”
21 11   Release “N”
20 12   Press “O”
21 12   Release “O”
20 17   Press “T”
21 17   Release “T”
20 08   Press “E”
21 08   Release “E”
20 13   Press “P”
21 13   Release “P”
20 04   Press “A”
21 04   Release “A”
20 07   Press “D”
21 07   Release “D”
20 28   Press return
21 28   Release return
ff      End of macro
```

 - 12:69 Ctrl+Alt+Del

```
22 01   Press left control
22 04   Press left alt
20 4c   Press delete
23 01   Release left control
23 04   Release left alt
21 4c   Release delete
ff      End of macro
```

 - 12:70 Move mouse in square loop

```
60 00 14 00 00   Move mouse pointer (+20, 0)
60 00 00 00 14   Move mouse pointer (0, +20)
60 ff ec 00 00   Move mouse pointer (−20, 0)
60 00 00 ff ec   Move mouse pointer (0, −20)
02               Repeat from beginning while the button is pressed
ff               End of macro
```

 - 12:81 Scroll the wheel while holding left shift (similar to 12:27 but with different syntax).

```
22 01               Press left control
24 01               Scroll mouse wheel up (+1)
61 01 90 12 89 -+   Wait 400ms (0x190), jump to 12:89 if the button is released
00              |   Do nothing (next instruction needs to be aligned)
24 01 <-----+   |   Scroll mouse wheel up (+1)
98          |   |   Delay ? (about 100ms)
45 12 86 ---+   |   Jump to 12:86 if the button is still pressed
23 01 <---------+   Release left control
ff                  End of macro
```

 - 12:95 Press “D” in an infinite loop (or I could not find how to stop it other than unplugging the mouse)

```
20 07   Press “D”
21 07   Release “D”
03      Repeat
ff      End of macro (do we ever get there?)
```

