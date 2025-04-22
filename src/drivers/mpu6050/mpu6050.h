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

class MPU6050 : public DriverIn<accle_data> {
    public:
        MPU6050(I2C *i2c, int addr);
        ~MPU6050();
        void update(accle_data& data) override;
    private:
        I2C *i2c;
        int addr;
        void read();
        void process();
        void reset();
        void read_accel(int16_t accel[3]);
};


#endif // MPU6050_H
#endif