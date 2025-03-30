#include "barometer.h"
#include <stdio.h>

Barometer::Barometer(SPI *spi, int cs) {
    printf("Barometer created\n");
    this->spi = spi;
}

Barometer::~Barometer(){
    printf("Barometer destroyed\n");
}

void Barometer::read(){
    printf("Barometer read\n");
}

void Barometer::update(core_flight_data& data){
    printf("Barometer update\n");
    data.altitude = 10;
}



