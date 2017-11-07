# weftlib
Arduino llibrary for [electrovibration](https://en.wikipedia.org/wiki/Electrovibration) actuation.  

Based gratefully off of this [library](https://github.com/yurikleb/DRV2667) by [Yurikleb](https://github.com/yurikleb/).  

## Required peripherals  
This library requires a TI [DRV2667](http://www.ti.com/lit/ds/symlink/drv2667.pdf) IC in order to work. Eventually, I intend to abstract the particular driver away and support other electrovibration techniques, like flyback converter and other driver ICs. Until then, this library works best on a Teensy 3.1/3.2 with a DRV2667 on its i2c bus.  

This library is specifically supporting the WEFT Engine board from the 2017 Hackaday Superconference.  

