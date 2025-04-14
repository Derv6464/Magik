#include "../../config.h"
#ifdef TESTING
#include "tester_core.h"

Tester_Core::Tester_Core(TestHandler* handler, char name){
    printf("Tester created with name %s\n", name);
    this->name = name;
    this-> hander = handler;
    printf("Tester created\n");
};

void Tester_Core::update(core_flight_data& data){

    switch (name) {
        case 'b':
            data.barometer.pressure = hander->last_baro_data.pressure;
            data.barometer.temperature = hander->last_baro_data.temperature;
            break;

        case 'a':
            data.acceleration.x = hander->last_accle_data.x;
            data.acceleration.y = hander->last_accle_data.y;
            data.acceleration.z = hander->last_accle_data.z;
            break;

        default:
            printf("Unknown sensor type\n");
    }
};

#endif