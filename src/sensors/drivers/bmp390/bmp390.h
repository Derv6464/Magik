#include "../../config.h"
#ifdef BARO_BMP390
#ifndef BMP390_H
#define BMP390_H
#include "bmp3_defs.h"
#include "bmp3.h"
#include "../../sensor.h"
#include "../../../tools/interfaces.h"
#include "../../data.h"
#include <stdio.h>

class BMP390 : public Sensor<core_flight_data> {
    public:
        BMP390(SPI *spi, int cs);
        ~BMP390();
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


#endif // BMP390_H
#endif