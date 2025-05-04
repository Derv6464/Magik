#include "gps.h"
#include <stdio.h>

GPS::GPS(UART *uart) {
    printf("GPS created, uart\n");
    #ifdef GPS_MAXM10S
        printf("GPS MAXM10S\n");
        gps = new MAXM10S(uart);
    #endif
    #ifdef GPS_PA
        printf("GPS PA\n");
        gps = new PA(uart);
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
