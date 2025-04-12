#include "../../config.h"
#ifdef TESTING
#ifndef TEST_INPUT_H
#define TEST_INPUT_H
#include "bmp3_defs.h"
#include "bmp3.h"
#include "../../sensor.h"
#include "../../../tools/interfaces.h"
#include "../../data.h"
#include <stdio.h>

class Tester : public Sensor<core_flight_data> {
    public:
        Tester(SPI *spi, int cs);
        void update(core_flight_data& data) override;
    private:
        SPI *spi;
        int cs;
        void read();
        void process();
        struct bmp3_dev baro;
        struct bmp3_settings settings;
        void bmp3_check_rslt(const char api_name[], int8_t rslt);
};


#endif // TEST_INPUT_H
#endif