#pragma once
//#define BARO_BMP390
//#define ACCEL_LSM6DS3
//#define ACCEL_ADXL345
#define ACCEL_MPU6050
//#define GPS_MAXM10S
//#define RADIO_SX1278
//#define LOG_SD

#define LOG_INTERNAL_FLASH
#define LOG_FLASH_OFFSET 1024*1024 
#define LOG_FLASH_SETTINGS_OFFSET 1024*512 
//#define TESTING


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
#define pyro_1 21
#define pyro_2 22
#define pyro_check_1 26
#define pyro_check_2 27

#define bt_setting_pin 2

#define read_data_delay 100 //ms 
#define send_data_delay 5000 //ms

namespace flight_settings {
    class FlightSettings {
    public:
        FlightSettings();
        ~FlightSettings();
        void init();
        void setFlightSettings(int main_deploy_height, int drouge_deploy_delay, int liftoff_threshold){
            this->main_deploy_height = main_deploy_height;
            this->drouge_deploy_delay = drouge_deploy_delay;
            this->liftoff_threshold = liftoff_threshold;
        };
        int getMainDeployHeight() { return main_deploy_height; }
        int getDrougeDeployDelay() { return drouge_deploy_delay; }
        int getLiftoffThreshold() { return liftoff_threshold; }
        void setMainDeployHeight(int height) { main_deploy_height = height; }
        void setDrougeDeployDelay(int delay) { drouge_deploy_delay = delay; }
        void setLiftoffThreshold(int threshold) { liftoff_threshold = threshold; }

    private:
        int main_deploy_height;
        int drouge_deploy_delay;
        int liftoff_threshold;
    };
}