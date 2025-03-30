#ifndef BAROMETER_H
#define BAROMETER_H

#include "sensor.h"
#include "tools/interfaces.h"
#include "data.h"

class Barometer : public Sensor<core_flight_data> {
    public:
        Barometer(SPI *spi, int cs);
        ~Barometer();
        void update(core_flight_data& data) override;
    private:
        SPI *spi;
        void read();
        void process();
};

#endif // BAROMETER_H