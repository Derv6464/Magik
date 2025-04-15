#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "sensor.h"
#include "tools/interfaces.h"
#include "data.h"
#include "config.h"
#include <stdio.h>
#ifdef TESTING
#include "drivers/test_input/tester_core.h"
#include "drivers/test_input/test_handler.h"
#endif



class Accelerometer{
    public:
        Accelerometer(SPI *spi, int cs);
        Accelerometer(I2C *i2c, int addr);
        Accelerometer(TestHandler* handler);
        ~Accelerometer();
        Sensor<core_flight_data>* getSensor() { return accelerometer; }
    private:
        Sensor<core_flight_data>* accelerometer;
};

#endif // ACCELEROMETER_H