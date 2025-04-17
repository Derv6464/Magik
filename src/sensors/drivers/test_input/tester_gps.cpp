#include "../../config.h"
#ifdef TESTING
#include "tester_gps.h"

Tester_Gps::Tester_Gps(TestHandler* handler) {
    this-> hander = handler;
    printf("Tester created\n");
};

void Tester_Gps::update(gps_data& data){
    data.latitude = hander->last_gps_data.latitude;
    data.longitude = hander->last_gps_data.longitude;
};

#endif