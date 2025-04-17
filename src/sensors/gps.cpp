#include "gps.h"
#include <stdio.h>

GPS::GPS(UART *uart) {
    #ifdef BARO_BMP390
        printf("Barometer BMP390\n");
        barometer = new BMP390(spi, cs);
    #endif
}

GPS::GPS(TestHandler* handler){
    #ifdef TESTING
        printf("GPS Tester\n");
        gps = new Tester_Gps(handler);
    #endif
    printf("GPS created\n");
}

GPS::~GPS(){
    if (gps != nullptr) {
        delete gps;
        gps = nullptr;
    }
    printf("GPS destroyed\n");
}
