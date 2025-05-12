#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "sensor.h"
#include "../drivers/driver_in.h"
#include "tools/interfaces.h"
#include "data.h"
#include "../config.h"
#include "FreeRTOS.h"
#include <stdio.h>
#include <cmath>

#ifdef TESTING
#include "../drivers/test_input/tester_accel.h"
#include "../drivers/test_input/test_handler.h"
#endif

#ifdef ACCEL_MPU6050
#include "../drivers/mpu6050/mpu6050.h"
#endif

class Accelerometer: public Sensor<core_flight_data> {
    public:
        Accelerometer(SPI *spi, int cs);
        Accelerometer(I2C_BASE *i2c, int addr);
        #ifdef TESTING
        Accelerometer(TestHandler* handler);
        #endif
        ~Accelerometer();
        void update(core_flight_data* data) override;
    private:
        DriverIn<accle_data>* accelerometer;
        float getVelocity(float last_velocity, float accel, float time);
};

#endif // ACCELEROMETER_H