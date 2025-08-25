# :pushpin: BMA180_RPi
A C program for interfacing the BMA180 Triple Axis Digital Accelerometer with a Raspberry Pi (RPi). 
This code was tested on the RPi 4 using the wiringPi library and the I2C interface for communication.

## BMA180 IC and Breakout Board
The following is the BMA180 IC with breakout board used.
[https://www.amazon.com/gp/product/B06XHNWBX4/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1]

## BMA180 Datasheet
[https://www.geeetech.com/Documents/BST-BMA180-DS000.pdf](https://www.geeetech.com/Documents/BST-BMA180-DS000.pdf)

## wiringPi Library
[http://wiringpi.com/](http://wiringpi.com/)

## PIN Configuration
The following table shows the pin configuration for connecting the BMA180 to the RPi.
```
| BMA180 |     RPi     |  Description |
|--------|-------------|--------------|
| 3V3    | 3V3         | 3.3V supply  |    
| SCL    | GPIO3 (SCL) | Serial Clock |   
| SDA    | GPIO2 (SDA) | Serial Data  |
| GND    | GND         | Ground  	    |
| CSB    | 3V3         |   -          |
| INT    | GPIO17      |   -          |
```
## Operation
Currently, the code can:
1.  Reset device
2.  Enable/disable write
3.  Get and set device bandwidth
4.  Get and set device range
5.  Get and set device mode configuration
6.  Enable/disable new data interrupts
7.  Enable/disable sample skipping

**Note:**  
This sensor is intended to be used in the development of a low-cost seismic data acquisition system. The code is not completed.

## :pray: Acknowledgements
* [https://www.electrodragon.com/bma180-three-axis-accelerometer-demo-test-with-arduino/](https://www.electrodragon.com/bma180-three-axis-accelerometer-demo-test-with-arduino/)
* [https://www.geeetech.com/wiki/index.php/BMA180_Triple_Axis_Accelerometer_Breakout](https://www.geeetech.com/wiki/index.php/BMA180_Triple_Axis_Accelerometer_Breakout)
* [https://github.com/derekmolloy/beaglebone](https://github.com/derekmolloy/beaglebone)

