#include "../../config.h"
#ifdef TESTING
#include "tester_baro.h"

Tester_Baro::Tester_Baro(TestHandler* handler){
    this-> hander = handler;
    printf("Tester created\n");
};

void Tester_Baro::update(baro_data& data){
    data.pressure = hander->last_baro_data.pressure;
    data.temperature = hander->last_baro_data.temperature;
};

#endif