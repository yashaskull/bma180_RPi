#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

#include "bma180.h"


/***************************************************************
 * bma180_soft_reset():
 * 
 * Perform a soft reset on the device.
 * 
 * fd: Device descriptor.
 * 
 * RESET: Register address used for soft reset.
 * 
 * 0xB6: Value written to register to perform soft reset
 * 
 * Return -1 on error.
 * 
 ***************************************************************/
int bma180_soft_reset(int fd)
{
    if (wiringPiI2CWriteReg8(fd, RESET, 0xB6) < 0)// soft reset
    {
	printf("Failed to soft reset device.\n");
	return -1;
    }
    printf("Device reset!.");
    usleep(100000); // 100 ms
    return 1;
}

/***************************************************************
 * bma180_enable_write():
 * 
 * Enable writing to registers on the device.
 * 
 * fd: Device descriptor.
 * 
 * CR0: Control regsiter 0 address value used for enabling write.
 *      bit 4 (ee_w) set to 1
 * 
 * 0x10: Value written to CR0 to enable write. 
 * 
 * Return -1 on error.
 * 
 ***************************************************************/
int bma180_enable_write(int fd)
{
    // enable write. Set bit4 (ee_w) = 1
    if (wiringPiI2CWriteReg8(fd, CR0, 0x10) < 0)
    {
	printf("Failed to enable write on the device.\n");
	return -1;
    }
    printf("Device write enabled!\n");
    usleep(100000); // 100 ms
    return 1;
}

/***************************************************************
 * bma180_disable_write():
 * 
 * Disable writing to registers on the device.
 * 
 * fd: Device descriptor.
 * 
 * CR0: Control regsiter 0 address used for disabling write.
 *      bit 4 (ee_w) set to 0
 * 
 * 0x00: Value written to CR0 to disable write. 
 * 
 * Return -1 on error.
 * 
 ***************************************************************/
int bma180_disable_write(int fd)
{
     // disable write. Set bit4 (ee_w) = 0
    if (wiringPiI2CWriteReg8(fd, CR0, 0x00) < 0)
    {
	printf("Failed to disable write on the device.\n");
	return -1;
    }
    printf("Device write disabled!\n");
    usleep(100000); // 100 ms
    return 1;
}


/***************************************************************
 * bma180_get_bw():
 * 
 * Get device bandwidth setting:
 * 	LPF: 10Hz, 20Hz, 40Hz, 75Hz, 150Hz, 300Hz, 600Hz, 1200Hz
 * 	HPF: 1Hz
 * 	BPF: 0.2Hz - 300Hz
 * 
 * fd: Device descriptor.
 * 
 * bits 7 6 5 4 of BW register 
 * 
 * Return -1 on error.
 * 
 ***************************************************************/
int bma180_get_bw(int fd)
{
    // get bw register address
    char bw_addr_val = wiringPiI2CReadReg8(fd, BW);
    if (bw_addr_val < 0)
    {
	printf("Error reading bandwidth register value.\n");
	return -1;
    }
    //wiringPiI2CWrite(fd, BW);
    //printf("%d\n", rv_bw);
    //char bw_addr_val = wiringPiI2CRead(fd);
    //printf("bw address: %d\n", bw_addr_val);
    
    // extract bits 7 6 5 4
    char bw_val = bw_addr_val & 0xF0;// bit 7 6 5 4
    
    // shift right by 4 bits
    bw_val = bw_val >> 4;
    //printf("bw (dec): %d\n", bw_val);
    
    // which freq setting does bw_val compare too.
    switch (bw_val)
    {
	// 10 Hz
	case bw_10Hz:
	    //printf("bw: 10 Hz\n");
	    return 10;
	// 20 Hz
	case bw_20Hz:
	    //printf("bw: 20 Hz\n");
	    return 20;
	// 40 Hz
	case bw_40Hz:
	    //printf("bw: 40 Hz\n");
	    return 40;
	// 75 Hz
	case bw_75Hz:
	    //printf("bw: 75 Hz\n");
	    return 75;
	// 150 Hz
	case bw_150Hz:
	    //printf("bw: 150 Hz\n");
	    return 150;
	// 300 Hz
	case bw_300Hz:
	    //printf("bw: 300 Hz\n");
	    return 300;
	// 600 Hz
	case bw_600Hz:
	    //printf("bw: 600 Hz\n");
	    return 600;
	// 1200 Hz
	case bw_1200Hz:
	    //printf("bw: 1200 Hz\n");
	    return 1200;
	// HPF 1 Hz
	case bw_HP_1Hz:
	    //printf("bw: HPF 1 Hz\n");
	    return 1;
	// BPF 0.2 Hz - 300 Hz
	case bw_BP_02Hz_300Hz:
	    //printf("bw: BPF 0.2 Hz - 300 Hz\n");
	    return 2;
	default:
	    printf("bw not found\n");
	    return 0;
    }
}



/*****************************************************************
 * bma180_set_bw():
 * 
 * Set bandwidth of the device.
 * 
 * fd: Device descriptor.
 * 
 * bw_value: Bandwidth value to set.
 * 	     Refer to enum bma180_bw{} in bma180.h for bw settings
 * 
 * bits 7 6 5 4 of BW register 
 * 
 * Return -1 on error.
 * 
 *****************************************************************/
int bma180_set_bw(int fd, char bw_value)
{
    
    if (bw_value == 0 || bw_value == 1 || bw_value == 2 || bw_value == 3 || bw_value == 4 || bw_value == 5 || bw_value == 6 || bw_value == 7|| bw_value == 8 || bw_value == 9)
    {
	// get bw reg addr value
	char bw_addr_val = wiringPiI2CReadReg8(fd, BW);
	if (bw_addr_val < 0)
	{
	    printf("Error reading bandwidth register address value.\n");
	    return -1;
	}
	// move bw value into bits 7 6 5 4
	char temp = bw_value << 4;
	// clear bits 7 6 5 4 from bw reg addr value
	bw_addr_val = bw_addr_val & 0x0F;
	temp = bw_addr_val | temp;
	
	// write new value to bw reg addr
	if (wiringPiI2CWriteReg8(fd, BW, temp) < 0) // need to check for errors
	{
	    printf("Error setting device bandwidth.\n");
	    return -1;
	}
	
	usleep(100000); // 100 ms
	return 1;
    }
    else
    {
	printf("Device does not support bw setting\n");
	return 0;
    }
}


/******************************************************************
 * bma180_get_range():
 * 
 * Get current range setting of the device:
 * 	+/- 1g, +/- 1.5g, +/- 2g, +/- 3g, +/- 4g, +/- 8g, +/- 16g
 * 
 * fd: Device descriptor.
 * 
 * bits 3 2 1 of RANGE Address
 * 
 * Return -1 on error.
 * 
 ******************************************************************/
float bma180_get_range(int fd)
{
    // get address value where range bits are set
    char range_addr_val = wiringPiI2CReadReg8(fd, RANGE);
    if (range_addr_val < 0)
    {
	printf("Error reading range register address value.\n");
	return -1;
    }
    //printf("range address: %d\n", range_addr_val);
    
    // get range.. bits 3 2 1
    char range_val = range_addr_val & 0x0E;
    
    // shift right by 1
    range_val = range_val >> 1;
    //printf("range val: %d\n", range_val);
    
    switch (range_val)
    {
	// +/- 1g
	case range_1g:
	    //printf("range: +/- 1g\n");
	    return 1.0;
	// +/- 1.5g
	case range_1_5g:
	    //printf("range: +/- 1.5g\n");
	    return 1.5;
	// +/- 2g
	case range_2g:
	    //printf("range: +/- 2g\n");
	    return 2.0;
	// +/- 3g
	case range_3g:
	    //printf("range: +/- 3g\n");
	    return 3.0;
	// +/- 4g
	case range_4g:
	    //printf("range: +/- 4g\n");
	    return 4.0;
	// +/- 8g Hz
	case range_8g:
	    //printf("range: +/- 8g\n);
	    return 8.0;
	// +/- 16 g
	case range_16g:
	    //printf("range: +/- 16g\n");
	    return 16.0;
	default:
	    printf("Range not found. \n");
	    return -1;
    }
}

/************************************************************************
 * bma180_set_range():
 * 
 * Set range of the device.
 * 
 * fd: Device descriptor.
 * 
 * range_value: Range value to set.
 * 	     Refer to enum bma180_range{} in bma180.h for range settings
 * 
 * bits 3 2 1 of RANGE Address
 * 
 * Return -1 on error.
 * 
 ************************************************************************/
int bma180_set_range(int fd, char range_value)
{
    if (range_value == 0 || range_value == 1 || range_value == 2 || range_value == 3 || range_value == 4 || range_value == 5 || range_value == 6)
    {
	// move range value into position.. bits 3 2 1
	char temp = range_value << 1;
	
	// get range addr value
	char range_addr_val = wiringPiI2CReadReg8(fd, RANGE);
	if (range_addr_val < 0)
	{
	    printf("Error reading range register address value.\n");
	    return -1;
	}
	
	range_addr_val = range_addr_val & 0xF1;
	
	temp = range_addr_val | temp;
	//printf("%d\n", temp);
	
	if (wiringPiI2CWriteReg8(fd, RANGE, temp) < 0)
	{
	    printf("Error setting device range.\n");
	    return -1;
	}
	
	usleep(100000); // 100 ms
	return 1;
    }
    else
    {
	printf("Device does not support range value setting\n");
	return 0;
    }
}


/******************************************************************
 * bma180_get_mode_config():
 * 
 * Get current mode configuration setting:
 * 	Low power or low noise
 * 
 * fd: Device descriptor.
 * 
 * bits 1 0 of MODE_CONFIG address
 * 
 * Return -1 on error.
 * 
 ******************************************************************/
int bma180_get_mode_config(int fd)
{
    // get address of register used for mode configuration
    char mode_config_addr_value = wiringPiI2CReadReg8(fd, MODE_CONFIG);
    //printf("%d\n", mode_config_addr);
    if (mode_config_addr_value < 0)
    {
	printf("Error reading mode config register value.\n");
	return -1;
    }
    
    // get bits 1 0, which is the mode configuration setting
    char mode_config_value = mode_config_addr_value & 0x03;
    
    switch (mode_config_value)
    {
	case mode_low_noise:
	    printf("Device configured in low noise mode.\n");
	    return 0;
	
	case mode_low_power:
	    printf("Device configured in low power mode.\n");
	    return 3;
	
	default:
	    printf("Could not read mode configuration.\n");
	    return 1;
    }
}


/************************************************************************
 * bma180_set_mode_config():
 * 
 * Set mode configuration of the device.
 * 	refer to enum bma180_mode_config{} in bma180.h for mode settings
 * 
 * fd: Device descriptor.
 * 
 * Return -1 on error.
 * 
 ************************************************************************/
int bma180_set_mode_config(int fd, char mode_config_value)
{
    char mode_config_addr_value = wiringPiI2CReadReg8(fd, MODE_CONFIG);
    if (mode_config_addr_value < 0)
    {
	printf("Error reading mode configuration register value.\n");
	return -1;
    }
    
    // set bits 1 0
    char temp = mode_config_addr_value | mode_config_value;
    
    if (wiringPiI2CWriteReg8(fd, MODE_CONFIG, temp) < 0)
    {
	printf("Error setting device mode config.\n");
	return -1;
    }
    
    usleep(100000); // 100 ms
    return 1;
}

/***************************************************************************************
 * enable_new_data_interrupt():
 * 
 * Enable new data interrupt.
 * Interrupt pin goes HIGH when new data is available.
 * Interrupt is non-latched so interrupt condition is reset by device when data is read.
 * 
 * fd: Device descriptor.
 * 
 * Return -1 on error.
 * 
 ***************************************************************************************/
int enable_new_data_interrupt(int fd)
{
    // set bit 1 of control register 3 to 1 to enable new data interrupt
    if (wiringPiI2CWriteReg8(fd, CR3, 0x02) < 0) // enable new data interrupt
    {
	printf("Error enabling new data interrupt.\n");
	return -1;
    }
    usleep(100000);
    return 1;
}

/***************************************************************************************
 * disable_new_data_interrupt():
 * 
 * Disable new data interrupt.
 * 
 * Writing 0x00 to CR3 will actually disable all interrupts.
 * 
 * fd: Device descriptor.
 * 
 * Return -1 on error.
 * 
 ***************************************************************************************/
int disable_new_data_interrupt(int fd)
{
    // Set bit 1 of CR3 to 0 to disable new data interrupt.
    // Actually, writing 0x00 to CR3 will actually disable all interrupts.
    if (wiringPiI2CWriteReg8(fd, CR3, 0x00) < 0) // disable new data interrupt
    {
	printf("Error disabling new data interrupt.\n");
	return -1;
    }
    usleep(100000);
    return 1;
}

/***************************************************************************************
 * enable_sample_skipping():
 * 
 * Enable sample skipping. Sample rate based on bandwidth setting.
 * Sample skipping used in relation to new data interrupt.
 * 
 * fd: Device descriptor.
 * 
 * Return -1 on error.
 * 
 ***************************************************************************************/
int enable_sample_skipping(int fd)
{
    char sample_skip_addr_value = wiringPiI2CReadReg8(fd, RANGE);
    if (sample_skip_addr_value < 0)
    {
	printf("Error reading sample skip address value.\n");
	return -1;
    }
    // set bit 0 of sample skip address value to 1
    sample_skip_addr_value = sample_skip_addr_value | 0x01;
    if (wiringPiI2CWriteReg8(fd, RANGE, sample_skip_addr_value) < 0)
    {
	printf("Error enabling device sample skipping.\n");
	return -1;
    }
    usleep(100000);
    return 1;
}

/***************************************************************************************
 * disable_sample_skipping():
 * 
 * Disable sample skipping. Sample rate based on bandwidth setting.
 * Sample skipping used in relation to new data interrupt.
 * 
 * fd: Device descriptor.
 * 
 * Return -1 on error.
 * 
 ***************************************************************************************/
int disable_sample_skipping(int fd)
{
    char sample_skip_addr_value = wiringPiI2CReadReg8(fd, RANGE);
    if (sample_skip_addr_value < 0)
    {
	printf("Error reading sample skip address value.\n");
	return -1;
    }
    // set bit 0 of sample skip address value to 0
    sample_skip_addr_value = sample_skip_addr_value | 0x00;
    if (wiringPiI2CWriteReg8(fd, RANGE, sample_skip_addr_value) < 0)
    {
	printf("Error disabling device sample skipping.\n");
	return -1;
    }
    usleep(100000);
    return 1;
}

