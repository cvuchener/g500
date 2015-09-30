Logitech HID Reports
====================

HID++
-----

Two input/output reports are used for Logitech’s HID++ protocol. There are the same for [HID++ 1.0](hidpp10.md) and HID++ 2.0:
  - Short report (7 bytes)
    - **Report ID**: 0x10
    - **Usage**: 0xFF000001
    - **Report Size**: 8
    - **Report Count**: 6
  - Long report (20 bytes)
    - **Report ID**: 0x11
    - **Usage**: 0xFF000002
    - **Report Size**: 8
    - **Report Count**: 19

DJ
--

DJ reports are contained in the same *Application* collection with usage 0xFF000004:
  - Short report (15 bytes)
    - **Report ID**: 0x20
    - **Usage**: 0xFF000041
    - **Report Size**: 8
    - **Report Count**: 14
  - Long report (32 bytes)
    - **Report ID**: 0x21
    - **Usage**: 0xFF000042
    - **Report Size**: 8
    - **Report Count**: 31
