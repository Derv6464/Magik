#include "../../config.h"
#ifdef ACCEL_MPU6050
#ifndef MPU6050_H
#define MPU6050_H
#include "../driver_in.h"
#include "../../tools/interfaces.h"
#include "../../sensors/data.h"
#include <stdio.h>
#include "FreeRTOS.h"

//most code from offical raspberry pi pico example
typedef enum {
    MPU6050_RANGE_2_G = 0,  ///< +/- 2g (default value)
    MPU6050_RANGE_4_G = 1,  ///< +/- 4g
    MPU6050_RANGE_8_G = 2,  ///< +/- 8g
    MPU6050_RANGE_16_G = 3, ///< +/- 16g
  } mpu6050_accel_range_t;


class MPU6050 : public DriverIn<accle_data> {
    public:
        MPU6050(I2C_BASE *i2c, int addr);
        ~MPU6050();
        void update(accle_data& data) override;
    private:
        I2C_BASE *i2c;
        int device_addr;
        void reset();
        void read_raw_accel(int16_t accel[3]);
        bool get_id();
        void update_range();
        void set_range(mpu6050_accel_range_t accel_range);
        mpu6050_accel_range_t accel_range{MPU6050_RANGE_2_G};
        uint8_t const MPU6050_ADDRESS = 0x68; // I2C address of the MPU6050
        uint8_t const MPU6050_ACCEL_OUT = 0x3B; // Register address for accelerometer X-axis high byte
        uint8_t const MPU6050_ACCEL_CONFIG = 0x1C; // Register address for accelerometer configuration
        uint8_t const MPU6050_ID_LOC = 0x75; // Register address for device ID
        uint8_t const MPU6050_ID = 0x68; // Expected device ID
    
};



#endif // MPU6050_H
#endif