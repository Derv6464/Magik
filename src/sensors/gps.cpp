#include "gps.h"
#include <stdio.h>

GPS::GPS(UART *uart) {
    #ifdef BARO_BMP390
        printf("Barometer BMP390\n");
        barometer = new BMP390(spi, cs);
    #endif
}

#ifdef TESTING
GPS::GPS(TestHandler* handler){
    printf("GPS Tester\n");
    gps = new Tester_Gps(handler);
   
    printf("GPS created\n");
}
#endif

GPS::~GPS(){
    if (gps != nullptr) {
        delete gps;
        gps = nullptr;
    }
    printf("GPS destroyed\n");
}

void GPS::update(secondary_flight_data* data) {
    gps_data new_data;
    gps->update(new_data);
    data->gps.latitude = new_data.latitude;
    data->gps.longitude = new_data.longitude;
}
