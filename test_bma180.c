
//BMA180 triple axis accelerometer sample code//
//www.geeetech.com//
//

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>
#include <stdint.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "bma180.h"

#define SOFT_RESET 0XB6
//
int offx = 31;  
int offy = 47;   
int offz = -23; 
//

static volatile int new_data;
static volatile int counter = 0;
void bma_interrupt(void)
{
    new_data = 1;
    counter ++;
    //printf("%d\n", counter);
}

int main()
{
    
    int fd;
    
    wiringPiSetup();
    
        
    pinMode(0, INPUT); // pin 17
    pinMode(2, OUTPUT);
    
    /**
    if (wiringPiISR(0, INT_EDGE_RISING, &bma_interrupt) < 0)
    {
	printf("error is\n");
    }*/
    
    fd = wiringPiI2CSetup(0x40);
    if (fd == -1)
    {
	printf("Failed to setup I2C device\n");
	return -1;
    }
    else
    {
	
	bma180_soft_reset(fd);
	bma180_enable_write(fd);
	
	bma180_set_mode_config(fd, mode_low_noise);
	
	enable_new_data_interrupt(fd);
	//wiringPiI2CWriteReg8(fd, 0x21, 0x02);// enable new data interrupt
	//sleep(1);
	
	enable_sample_skipping(fd);
	
	// sample skipping
	//char addr = wiringPiI2CReadReg8(fd, 0x35);
	//addr = addr | 0x01;
	//wiringPiI2CWriteReg8(fd, 0x35, addr);
	//sleep(1);
	
	bma180_set_bw(fd, bw_150Hz);
	bma180_set_range(fd, range_1g);
	
	int gg = 8192;
	while(1)
	{
	    /**if (new_data)*/ if (digitalRead(0) == HIGH)
	    {
		//new_data = 0;
		
		short data_x = wiringPiI2CReadReg8(fd, 0x03); // msb
		data_x = data_x << 8 | wiringPiI2CReadReg8(fd, 0x02);// lsb
		data_x = data_x >> 2;
		printf("%d  %f", data_x, (float)data_x/gg);
		
		short data_y = wiringPiI2CReadReg8(fd, 0x05); // msb
		data_y = data_y << 8 | wiringPiI2CReadReg8(fd, 0x04);
		data_y = data_y >> 2;
		printf("  %d  %f", data_y, (float)data_y/gg);
		
		short data_z = wiringPiI2CReadReg8(fd, 0x07); // msb
		data_z = data_z << 8 | wiringPiI2CReadReg8(fd, 0x06);
		data_z = data_z >> 2;
		printf("  %d  %f\n", data_z, (float)data_z/gg);
		
		/**
		wiringPiI2CWrite(fd, DATA);
		unsigned char result[5];
		int i = 0;
		while (i < 6)
		{
		    result[i] = wiringPiI2CRead(fd);
		    i++;
		}
		int x = ((result[0] | result[1] << 8) >> 2) + offx;
		//printf("%d", x);
		//printf("  ");
		
		int y = ((result[2] | result[3] << 8) >> 2) + offy;
		//printf("%d", y);
		//printf("  ");
		
		int z = ((result[4] | result[5] << 8) >> 2) + offz;
		//printf("%d\n", z);*/
	    }
	}
	
	//digitalWrite(27, LOW);
	/**
	while (1)
	{
	    digitalWrite(27, HIGH);
	    sleep(1);
	    digitalWrite(27, LOW);
	    sleep(1);
	}*/
	
	/**
	 * Get accel bw setting
	 * 
	 * if bw == 1.... HPF 1 Hz
	 * if bw == 2.... BPF 0.2 Hz - 300 Hz
	 * if bw == -1.... Error reading bw
	 * 
	 */
	//int bw = bma180_get_bw(fd);
	//printf("%d\n", bw);
	
	//float range = bma180_get_range(fd);
	//printf("%f\n", range);
	
	// Get accel bw register addr
	//printf("%d\n", bma180_get_bw_reg_addr(fd));
	
	/**
	 * Set accel bw
	 * 
	 * 0 -- 10 Hz
	 * 1 -- 20 Hz
	 * 2 -- 40 Hz
	 * 3 -- 75 Hz
	 * 4 -- 150 Hz
	 * 5 -- 300 Hz
	 * 6 -- 600 Hz
	 * 7 -- 1200 Hz
	 * 8 -- HPF 1 Hz
	 * 9 -- BPF 0.2 Hz - 300 Hz
	 *
	 */
	//bma180_set_bw(fd, 2);
	
	///////////////////////////////////////////////////////////////////////////////
	//int rv_reset = wiringPiI2CWriteReg8(fd, RESET, 0xB6);// soft reset
	//usleep(100000); // 100 ms
	//int rv_pwr = wiringPiI2CWriteReg8(fd, PWR, 0x10); // enable write
	//printf("%d    %d\n", rv_reset, rv_pwr);
    
    
	//char a = wiringPiI2CReadReg8(fd, 0X07);
	//printf("%d\n", a);
	
    
	/**

	while (1)
	{
	    wiringPiI2CWrite(fd, DATA);
	    unsigned char result[5];
	    int i = 0;
	    while (i < 6)
	    {
		result[i] = wiringPiI2CRead(fd);
		i++;
	    }
	    int x = ((result[0] | result[1] << 8) >> 2) + offx;
	    printf("%d", x);
	    printf("  ");
	    
	    int y = ((result[2] | result[3] << 8) >> 2) + offy;
	    printf("%d", y);
	    printf("  ");
	    
	    int z = ((result[4] | result[5] << 8) >> 2) + offz;
	    printf("%d\n", z);
	    /**
	    wiringPiI2CWrite(fd, DATA);
	    short data_x = wiringPiI2CRead(fd);
	    data_x+= wiringPiI2CRead(fd) << 8;
	    float x = data_x/4096.0;
	    printf("%d  %f", data_x, x);
	    printf("   ");
	    
	    short data_y = wiringPiI2CRead(fd);
	    data_y+= wiringPiI2CRead(fd) << 8;
	     float y = data_y/4096.0;
	    printf("%d  %f", data_y, y);
	    printf("   ");
	    
	    short data_z = wiringPiI2CRead(fd);
	    data_z += wiringPiI2CRead(fd) << 8;
	     float z = data_z/4096.0;
	    printf("%d  %f\n", data_z, z);*/
	
   }
    return 1;
} 

/**
void setup() 
{ 
 Serial.begin(9600); 
 Wire.begin(); 
 Serial.println("Demo started, initializing sensors"); 
 AccelerometerInit(); 
 Serial.println("Sensors have been initialized"); 
} 
//
void AccelerometerInit() 
//
{ 
 byte temp[1];
 byte temp1;
  //
  writeTo(BMA180,RESET,0xB6);
  //wake up mode
  writeTo(BMA180,PWR,0x10);
  // low pass filter,
  readFrom(BMA180, BW,1,temp);
  temp1=temp[0]&0x0F;
  writeTo(BMA180, BW, temp1);   
  // range +/- 2g 
  readFrom(BMA180, RANGE, 1 ,temp);  
  temp1=(temp[0]&0xF1) | 0x04;
  writeTo(BMA180,RANGE,temp1);
}
//
void AccelerometerRead() 
{ 
 // read in the 3 axis data, each one is 14 bits 
 // print the data to terminal 
 int n=6;
 byte result[5];
 readFrom(BMA180, DATA, n , result);
 
 int x= (( result[0] | result[1]<<8)>>2)+offx ;
 float x1=x/4096.0;
 Serial.print("x=");
 Serial.print(x1);
 Serial.print("g"); 
 //
 int y= (( result[2] | result[3]<<8 )>>2)+offy;
 float y1=y/4096.0;
 Serial.print(",y=");
 Serial.print(y1);
 Serial.print("g"); 
 //
 int z= (( result[4] | result[5]<<8 )>>2)+offz;
 float z1=z/4096.0;
 Serial.print(",z=");
 Serial.print(z1);
 Serial.println("g"); 
}
//
void loop() 
{ 
 AccelerometerRead(); 
 delay(300); // slow down output   
}
//
//---------------- Functions--------------------
//Writes val to address register on ACC
void writeTo(int DEVICE, byte address, byte val) 
{
  Wire.beginTransmission(DEVICE);   //start transmission to ACC
  Wire.write(address);               //send register address
  Wire.write(val);                   //send value to write
  Wire.endTransmission();           //end trnsmisson
}
//reads num bytes starting from address register in to buff array
 void readFrom(int DEVICE, byte address , int num ,byte buff[])
 {
 Wire.beginTransmission(DEVICE); //start transmission to ACC
 Wire.write(address);            //send reguster address
 Wire.endTransmission();        //end transmission
 
 Wire.beginTransmission(DEVICE); //start transmission to ACC
 Wire.requestFrom(DEVICE,num);  //request 6 bits from ACC
 
 int i=0;
 while(Wire.available())        //ACC may abnormal
 {
 buff[i] =Wire.read();        //receive a byte
 i++;
 }
 Wire.endTransmission();         //end transmission
 }
 */
