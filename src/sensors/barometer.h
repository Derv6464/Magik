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

class Barometer{
    public:
        Barometer(SPI *spi, int cs);
        ~Barometer();
        Sensor<core_flight_data>* getSensor() { return barometer; }
    private:
        Sensor<core_flight_data>* barometer;
};

#endif // BAROMETER_H