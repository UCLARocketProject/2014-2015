# Libraries

All mission-specific software libraries are held here. 

A list:

* `RadioHead` is a (as-of-yet) vanilla clone of the 
[RadioHead library](http://www.airspayce.com/mikem/arduino/RadioHead/), 
which has support for many radios and microprocessors. 

* `URP_LSM303` is a custom version of the 
[Adafruit library](https://github.com/adafruit/Adafruit_LSM303)
for our LSM303 accelerometer. We've tweaked it for higher
data resolution by modifying a few registers. 

* `serialtoplot` is our Python library for plotting data
received from an Arduino-radio pair over serial USB.
This one is writen (with love) from scratch. 

* More to come! 
