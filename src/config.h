#pragma once

//#define TESTING
#define TEST_RIG

#define read_data_delay 50 //ms 
#define send_data_delay 5000 //ms

//#define BARO_BMP390
//#define ACCEL_LSM6DS3
//#define ACCEL_ADXL345
#define ACCEL_MPU6050
//#define GPS_MAXM10S
//#define RADIO_SX1278
//#define LOG_SD

#ifdef TEST_RIG
#define BARO_BMP390
//#define ACCEL_LSM6DSOX
//#define GPS_PA
//#define RADIO_RM95W

#endif


#define LOG_INTERNAL_FLASH
#define LOG_FLASH_OFFSET 1024*1024 
#define SETTINGS_OFFSET 1024*512 

//I2C bus for high g accelerometer
//#define sda_0 0
//#define scl_0 1

//I2C bus for accelerometer
#define sda_0 4
#define scl_0 5

//SPI bus for barometer and low g accelerometer/IMU
#define sck_1 10
#define mosi_1 11
#define miso_1 12
#define cs_baro 13
#define cs_imu 5

//SPI bus for LoRa
#define sck_0 18
#define mosi_0 19
#define miso_0 16
#define cs_lora 17
#define rst_lora 20

#define neopixel 14
#define buzzer 15

//UART bus for GPS
//#define tx_1 8
//#define rx_1 9
#define rst_gps 7

#ifdef TESTING
//UART bus for testing
#define tx_1 8
#define rx_1 9

#endif

//Pyro channels
#define pyro_en 6
#define pyro_1 26
#define pyro_2 27
//#define pyro_check_1 26
//#define pyro_check_2 27

#define bt_setting_pin 2

