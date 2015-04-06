Memory
======

The memory is divided in 19 pages of 512 bytes each. Some pages behave differently than others.

Temporary memory (400 first bytes of page 0)
--------------------------------------------

The first 400 bytes of page 0 is always filled with zeros when plugging the mouse, so writing here will not modified the long-term state of the mouse.

It can be used for storing temporary profiles, when managing them from software. LGS also uses it as a intermediary buffer for writing before writing to the persistent memory

When writing to this memory, new values replace the old ones.


Unknown read-only data (112 last bytes of page 0)
-------------------------------------------------

The end of page 0 is not writable and already contains data when plugging the mouse.


Persistent memory (pages 1–16)
------------------------------

Data written in the pages 1 to 16 will persist after unplugging the mouse. Thus the mouse can store 8kb of data.

It can be used to store permanent profiles, macros or any metadata you want.

When wirting to this memory, new values are AND’ed over the old ones, so you need to feel the page with FF before writing.

Page 2 is a special page as it contains the profile that the mouse load when it is plugged. It is preferable to keep a sane profile in it.


Read-only profiles (pages 17 and 18)
------------------------------------

Page 17 and 18 each contain a profile. They are not the default profile. Page 18 also contains some useful macros.
