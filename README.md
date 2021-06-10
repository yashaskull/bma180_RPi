# bma180_rpi

A simple C program for interfacing bma180 triple axis digital acceleromter with a RPi 4 using the wiringPi library. I used the I2C interface for communication.

This is the breakout board I used:   
https://www.amazon.com/gp/product/B06XHNWBX4/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1

I developed this code with help from:  
https://www.electrodragon.com/bma180-three-axis-accelerometer-demo-test-with-arduino/
https://www.geeetech.com/wiki/index.php/BMA180_Triple_Axis_Accelerometer_Breakout  
https://github.com/derekmolloy/beaglebone

WiringPi:  
http://wiringpi.com/

BMA180 Datasheet:  
https://www.geeetech.com/Documents/BST-BMA180-DS000.pdf

PIN Configuration:  
Device<----->RPi  
3V3<-------->3V3  
SCL<-------->GPIO3(SCL)  
SDA<-------->GPIO2(SDA)  
GND<-------->Ground  
CSB<-------->3V3  
INT<-------->GPIO17

Currently, the code can:  
1.  Reset device
2.  Enable/disable write
3.  Get and set device bandwidth
4.  Get and set device range
5.  Get and set device mode configuration
6.  Enable/disable new data interrupts
7.  Enable/disable sample skipping

NOTE:
I plan to use this sensor for a seismic data acquisition system. The code is not complete and is in no way perfect. So feel free to use or not use it, make it better etc. I am just happy I am able to contribute to the opensource community :)
