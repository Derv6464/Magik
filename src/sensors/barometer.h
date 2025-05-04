#ifndef BAROMETER_H
#define BAROMETER_H

#include "sensor.h"
#include "../drivers/driver_in.h"
#include "tools/interfaces.h"
#include "data.h"
#include "../config.h"
#include <stdio.h>
#include <cmath>

#ifdef BARO_BMP390
#include "../drivers/bmp390/bmp390.h"
#endif
#ifdef TESTING
#include "../drivers/test_input/tester_baro.h"
#include "../drivers/test_input/test_handler.h"
#endif

class Barometer: public Sensor<core_flight_data> {
    public:
        Barometer(SPI *spi, int cs);
        #ifdef TESTING
        Barometer(TestHandler* handler);
        #endif
        ~Barometer();
        void update(core_flight_data* data) override;
    private:
        DriverIn<baro_data>* barometer;
        float getAltitude(float pressure);
        int const sea_level_pressure = 101325;
};

#endif // BAROMETER_H