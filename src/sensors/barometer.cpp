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
        barometer = new BMP390(spi, cs);
    #endif
}

#ifdef TESTING
Barometer::Barometer(TestHandler* handler){
   
        printf("Barometer Tester\n");
        barometer = new Tester_Baro(handler);
    
    printf("Barometer created\n");
}
#endif

Barometer::~Barometer(){
    if (barometer != nullptr) {
        delete barometer;
        barometer = nullptr;
    }
    printf("Barometer destroyed\n");
}

void Barometer::update(core_flight_data* data) {
    baro_data baro_data;
    barometer->update(baro_data);
    data->barometer.pressure = baro_data.pressure;
    data->barometer.temperature = baro_data.temperature;
    data->barometer.altitude = getAltitude(baro_data.pressure);
}

float Barometer::getAltitude(float pressure) {
    // Formula from BMP180 datasheet, given temp = 15C and sea level pressure = 1013.25hPa
    //printf("Pressure: %d\n", pressure);
    return 44330.0 * (1.0 - pow(pressure / sea_level_pressure, 0.1903));
}