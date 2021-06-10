#ifndef BMA180_H_INCLUDED
#define BMA180_H_INCLUDED


#define BMA180 0x40 // address of the accelerometer
#define RESET 0x10 // soft reset register address
#define CR0 0x0D // ctrl_reg_0 address. Used to enable write (bit 4 = ee_w)
#define BW 0X20 // bandwidth register address.
#define RANGE 0X35 // Used to get or set accelerometer range.
#define DATA 0x02 // acc_x_lsb register address.
#define MODE_CONFIG 0x30 // register address used for changing operation mode. bit 1 0
#define CR3 0X21 //register used for interrupts


enum bma180_bw
{
    bw_10Hz = 0,
    bw_20Hz = 1,
    bw_40Hz = 2,
    bw_75Hz = 3,
    bw_150Hz = 4,
    bw_300Hz = 5,
    bw_600Hz = 6,
    bw_1200Hz = 7,
    bw_HP_1Hz = 8,
    bw_BP_02Hz_300Hz = 9
};

enum bma180_range
{
    range_1g = 0,
    range_1_5g = 1,
    range_2g = 2,
    range_3g = 3,
    range_4g = 4,
    range_8g = 5,
    range_16g = 6,
};

enum bma180_mode_config
{
    mode_low_noise = 0,
    mode_low_power = 3
};



int bma180_soft_reset(int fd);
int bma180_enable_write(int fd);
int bma180_disable_write(int fd);

// digital filter bw
int bma180_get_bw(int fd);
int bma180_set_bw(int fd, char bw_value);

// range 
float bma180_get_range(int fd);
int bma180_set_range(int fd, char range_value);

// mode config
int bma180_get_mode_config(int fd);
int bma180_set_mode_config(int fd, char mode_config_value);

// interrupt
int enable_new_data_interrupt(int fd);
int disable_new_data_interrupt(int fd);

// sample skipping
int enable_sample_skipping(int fd);
int disable_sample_skipping(int fd);

#endif // BMA180_H_INCLUDED
