#include "../../config.h"
#ifdef TESTING
#include "tester_accel.h"

Tester_Accel::Tester_Accel(TestHandler* handler){
    this-> hander = handler;
    printf("Tester created\n");
};

void Tester_Accel::update(accle_data& data){
    data.x = hander->last_accle_data.x;
    data.y = hander->last_accle_data.y;
    data.z = hander->last_accle_data.z;
};

#endif