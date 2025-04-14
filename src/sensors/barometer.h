#ifndef BAROMETER_H
#define BAROMETER_H

#include "sensor.h"
#include "tools/interfaces.h"
#include "data.h"
#include "config.h"
#include <stdio.h>

#ifdef BARO_BMP390
#include "drivers/bmp390/bmp390.h"
#endif
#ifdef TESTING
#include "drivers/test_input/tester_core.h"
#include "drivers/test_input/test_handler.h"
#endif

class Barometer{
    public:
        Barometer(SPI *spi, int cs);
        Barometer(TestHandler* handler);
        ~Barometer();
        Sensor<core_flight_data>* getSensor() { return barometer; }
    private:
        Sensor<core_flight_data>* barometer = nullptr;
};

#endif // BAROMETER_H