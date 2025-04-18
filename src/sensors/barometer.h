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
        Barometer(TestHandler* handler);
        ~Barometer();
        void update(core_flight_data& data) override;
    private:
        DriverIn<baro_data>* barometer = nullptr;
        float getAltitude(float pressure);
        float const sea_level_pressure = 1013.25F;
};

#endif // BAROMETER_H