# USB to USB keyboard protocol converter (with Vial Support)

A small device to connect between your USB keyboard and your PC that makes (almost)
every TKL (or smaller) keyboard fully programmable, now with Vial support. Original
code from the [TMK firmware](https://github.com/tmk/tmk_keyboard/tree/master/converter/usb_usb).
[Original QMK port](https://github.com/qmk/qmk_firmware/tree/master/keyboards/converter/usb_usb/)
by [Balz Guenat](https://github.com/BalzGuenat). Adjusted for Vial support by
[Putra Sattvika](https://github.com/putrasattvika).

Keyboard Maintainer: [Putra Sattvika](https://github.com/putrasattvika)
Hardware Supported: [Arduino Leonardo + USB Host Shield](https://geekhack.org/index.php?topic=80421.0), maybe more
Hardware Availability: Self-built

Make & flash example for this keyboard (after setting up your build environment):

```
$ qmk compile -kb converter/usb_usb_vial/leonardo -km vial
$ qmk flash -kb converter/usb_usb_vial/leonardo -km vial
```

See [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools)
then the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide)
for more information.

Note that you have to choose the right hardware variant as your subproject,
otherwise you will probably have issues.


## Limitations

This variant of the USB-to-USB converter only supports TKL or smaller keyboards
because we're limited by atmega32u4's EEPROM size. Support for multiple keyboards
is removed, but I'm not sure how the Leonardo board is supposed to have multiple
keyboards connected in the first place.

The limitations of the original port still applies, e.g. it only supports
'HID Boot protocol' (i.e. no NKRO).


## Resources
- [The original QMK port of the converter by Balz Guenat](https://github.com/qmk/qmk_firmware/tree/master/keyboards/converter/usb_usb/)
