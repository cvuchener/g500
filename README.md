G500
====

This repository contains the documentation and code for the Logitech G500 specific HID protocol for profile configuration. The code is not meant to be an user-firendly configuration software but an example of how to interact with the mouse.

What I know of the configuration protocol was guessed from usb trace from Logitech Gaming Software (LGS) on Windows (thanks to USBPcap) and previous work from [Andreas Schneider](http://blog.cryptomilk.org/2011/02/22/logitech-linux-mouse-support/) and [Vladyslav Shtabovenko](https://github.com/vsht/g500-control/).

I cannot give you any guarantee that the code or the documentation in this repository is correct and will not break your mouse. But I think that the worst that could happen is having corrupted profiles or macros in memory, and if you do not know how to fix that, LGS does: just plug it into a Windows computer and launch LGS. Any other bad state that I have seen is not persistent and is reset when unplugging the mouse. Still it is a good idea to start your hacking by dumping the whole memory into backup files.

I consider the documentation an important part of this project. Any contribution to it is welcome, be it about adding content and explanations or fixing my bad english.


Compiling
---------

There is a simple Makefile in the src directory. Compilation options can be changed in the first lines.

There is no dependencies other than the standard C library and POSIX system calls (open, read and write).


hidraw
------

These tools use Linux’s hidraw interface to communicate with the mouse. The G500 creates two hidraw devices, you must use the second one to configure the mouse.

You need read and write access to the hidraw device, so either run the commands as root or change the permission of the device. Here is a sample udev rule file that I use to add read/write permissions to users in a *mouseconfig* group:
```
ATTRS{idVendor}=="046d", ATTRS{idProduct}=="c068", GOTO="logitech_mouse"
GOTO="logitech_end"

LABEL="logitech_mouse"
KERNEL=="hidraw*", SUBSYSTEM=="hidraw", ATTRS{bInterfaceNumber}=="01", GROUP="mouseconfig", MODE="0660"

LABEL="logitech_end"
```


Tools
-----

I made these tools mainly to help me hacking and testing my ideas, but they can be used to configure the mouse while waiting for a proper application.

 - `g500-dump-page /dev/hidrawN pagenumber`
   Read the page *pagenumber* from the mouse memory and write it to *stdout*.
 - `g500-read-block /dev/hidrawN pagenumber offset length`
   A more general purpose version of g500-dump-page. Only read *length* bytes of data from page *pagenumber* at offset *offset* (the offset is counting 16 bits words not bytes).
 - `g500-edit-profile command...` 
   Read a profile from *stdin* and write the modified profile to *stdout*. See the usage for details on the (too) many editing commands.
 - `g500-fill-page /dev/hidrawN pagenumber`
   Fill the page *pagenumber* with `FF` bytes.
 - `g500-mem-op [options] /dev/hidrawN`
   A general purpose command for testing the memory operations commands.
 - `g500-parse-macro`[offset]
   Parse the macro from *stdin* (at most 512 bytes). A whole page can be passed while specifying the offset.
 - `g500-parse-profile`
   Parse the profile from *stdin*.
 - `g500-set-profile /dev/hidrawN default|disable|pagenumber`
   Set the current profile to the one in page *pagenumber*.
 - `g500-write-block /dev/hidrawN len [pagenumber] [offset]`
   Write *len* bytes of data from *stdin* in page *pagenumber* at offset *offset* (default are 0). This command AND data if the page is not 0, use g500-fill-page before writing in other pages.
 - `g500-write-page /dev/hidrawN pagenumber`
   Overwrite the whole page with data from *stdin*.
 - `g500-write-page-lgs /dev/hidrawN pagenumber`
   Same as g500-write-page but using the same method as LGS.

A combination of these commands can be used in shell script to change the profile. For example, to temporarily change the middle side button to work as wheel click:
```
./g500-dump-page /dev/hidraw1 2 | ./g500-edit-profile bind 5 mouse 2 | ./g500-write-block /dev/hidraw1 78
./g500-set-profile /dev/hidraw1 0
```


Licenses
--------

The source code (in the src directory) is licensed under [GNU GPL](http://www.gnu.org/licenses/gpl.html). The documentation (in the doc directory) is licensed under [Creative Commons Attribution-ShareAlike 3.0](http://creativecommons.org/licenses/by-sa/3.0/).
