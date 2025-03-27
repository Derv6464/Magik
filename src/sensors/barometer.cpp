#include "barometer.h"

Barometer::Barometer(){
    printf("Barometer Constructor\n");
    //initialize();
}

Barometer::~Barometer(){
    printf("Barometer Destructor\n");
}

int Barometer::get_data(){
    printf("Barometer Get Data\n");
    return 0;
}

void Barometer::process(){
    printf("Barometer Process\n");
}

void Barometer::read(){
    printf("Barometer Read\n");
}
