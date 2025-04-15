#include "../../config.h"
#ifdef TESTING
#include "tester_sec.h"

Tester_Sec::Tester_Sec(TestHandler* handler, char name){
    printf("Tester created with name %s\n", name);
    this->name = name;
    this-> hander = handler;
    printf("Tester created\n");
};

void Tester_Sec::update(secondary_flight_data& data){
    switch (name) {
        case 'g':
            data.gps.latitude = hander->last_gps_data.latitude;
            data.gps.longitude = hander->last_gps_data.longitude;
            break;

        default:
            printf("Unknown sensor type\n");
    }
};

#endif