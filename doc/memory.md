Memory
======

The memory is divided in 19 pages of 512 bytes each. Some pages behave differently than others.

Memory organization
-------------------

### Temporary memory (400 first bytes of page 0)

The first 400 bytes of page 0 is always filled with zeros when plugging the mouse, so writing here will not modified the long-term state of the mouse.

It can be used for storing temporary profiles, when managing them from software.

When writing to this memory, new values replace the old ones.


### Unknown read-only data (112 last bytes of page 0)

The end of page 0 is not writable and already contains data when plugging the mouse. Some of this data is variable.


### Persistent memory (pages 1–16)

Data written in the pages 1 to 16 will persist after unplugging the mouse. Thus the mouse can store 8kb of data.

It can be used to store permanent profiles, macros or any metadata you want.

When writing to this memory, new values are AND’ed over the old ones, so you need to feel the page with FF before writing.

Page 2 is a special page as it contains the profile that the mouse load when it is plugged. It is preferable to keep a sane profile in it.


### Read-only profiles (pages 17 and 18)

Page 17 and 18 each contain a profile. They are not the default profile. Page 18 also contains some useful macros.


Logitech Gaming Software (LGS) use of memory
--------------------------------------

### End of page marker

At the end of each persistent memory page, LGS write a 16 bits integer for an unknown purpose. Not writing it does not prevent the profile from working.


### Page writing

LGS does not directly write to the persistent memory but use the temporary memory as an intermediary buffer. Steps are:
 1. Write the first 256 bytes in temporary memory.
 2. Fill the destination page with 0xFF.
 3. Copy 256 bytes from page 0 to the destination page.
 4. Write the last 256 bytes in temporary memory.
 5. Copy 256 bytes from page 0 to the destination page at offset 0x80.


### Metadata

After the profile on page 2, LGS stores the string “LGS02” and then the macro names in fixed length strings of 16 bits characters.


### Macros

LGS writes the first macro at the beginning of page 7 and continue on the next pages if it needs more space. Macros spanning over multiple pages must jump over the end of page marker.
