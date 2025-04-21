#include "test_handler.h"


TestHandler::TestHandler(UART* serial){
    printf("TestHandler created\n");
    this->serial = serial;
}

TestHandler::~TestHandler(){
    printf("TestHandler destroyed\n");
    if (serial != nullptr) {
        delete serial;
        serial = nullptr;
    }
    printf("TestHandler destroyed\n");
};

void TestHandler::split_data(){
    char buffer;
    serial->read(&buffer, 1);
    //printf("Buffer: %c\n", buffer);
    switch (buffer){
        case 'b':
            char baro_buffer[8];
            serial->read(baro_buffer, 8);
            last_baro_data.temperature = (*(int*)&baro_buffer[0])/100.00f;
            last_baro_data.pressure = *(int*)&baro_buffer[4];
            //printf("Driver: Barometer data: %d %f\n", last_baro_data.pressure, last_baro_data.temperature);
            break;

        case 'a':
            char accle_buffer[12];
            serial->read(accle_buffer, 12);
            last_accle_data.x = *(float*)&accle_buffer[0];
            last_accle_data.y = *(float*)&accle_buffer[4];
            last_accle_data.z = *(float*)&accle_buffer[8];
            //printf("Driver: Accelerometer data: %f %f %f\n", last_accle_data.x, last_accle_data.y, last_accle_data.z);
            break;

        case 'g':
            char gps_buffer[8];
            serial->read(gps_buffer, 16);
            last_gps_data.latitude = *(double*)&gps_buffer[0];
            last_gps_data.longitude = *(double*)&gps_buffer[8];
            //printf("Driver: GPS data: %lf %lf \n", last_gps_data.latitude, last_gps_data.longitude);
            break;
        
        default:
            printf("Unknown sensor type\n");
            break;
    }
};


