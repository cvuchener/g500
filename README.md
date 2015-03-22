# g500

This repository contains the documentation and code for g500 specific HID protocol for profile configuration. The code is not meant to be a configuration software but an example of how to interact with the mouse.

What I know of the configuration protocol was guessed from usb trace from Logitech Gaming Software (LGS) on Windows (thanks to USBPcap) and previous work from [Andreas Schneider](http://blog.cryptomilk.org/2011/02/22/logitech-linux-mouse-support/) and [Vladyslav Shtabovenko](https://github.com/vsht/g500-control/).

I cannot give you any guarantee that the code or the documentation in this repository is correct and will not break your mouse. But I think that the worse that could happen is having corrupted profiles or macros in memory, and if you do not know how to fix that, LGS does: just plug it into a Windows computer and launch LGS. Any other bad state that I have seen is not persistent and is reset when unplugging the mouse. Still it is a good idea to start your hacking by dumping the whole memory into backup files.
