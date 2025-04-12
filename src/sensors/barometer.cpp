#include "barometer.h"
#include <stdio.h>

Barometer::Barometer(SPI *spi, int cs) {
    printf("Barometer created\n");
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(cs);
    gpio_set_dir(cs, GPIO_OUT);
    gpio_put(cs, 1);
    // Make the CS pin available to picotool
    bi_decl(bi_1pin_with_name(cs, "SPI CS BARO"));

    #ifdef BARO_BMP390
        printf("Barometer BMP390\n");
        BMP390 baro(spi, cs);
        barometer = &baro;
    #endif
    printf("Barometer created\n");
}

Barometer::~Barometer(){
    if (barometer != nullptr) {
        delete barometer;
        barometer = nullptr;
    }
    printf("Barometer destroyed\n");
}

