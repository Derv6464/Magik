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

    printf("Buffer: %c\n", buffer);
    switch (buffer){
        case 'b':
            char baro_buffer[8];
            serial->read(baro_buffer, 8);
            printf("size: %d\n", sizeof(baro_data));
            last_baro_data.temperature = (*(int*)&baro_buffer[0])/100;
            last_baro_data.pressure = *(int*)&baro_buffer[4];
            printf("Barometer data: %d %f\n", last_baro_data.pressure, last_baro_data.temperature);
            break;

        case 'a':
            char accle_buffer[12];
            serial->read(accle_buffer, 12);
            last_accle_data.x = *(int*)&last_accle_data.x;
            last_accle_data.y = *(int*)&last_accle_data.y;
            last_accle_data.z = *(int*)&last_accle_data.z;
            printf("Accelerometer data: %d %d %d\n", last_accle_data.x, last_accle_data.y, last_accle_data.z);
            break;

        case 'g':
            serial->read((char*)&last_gps_data, sizeof(gps_data));
            printf("GPS data: %f %f %f %f %d\n", last_gps_data.latitude, last_gps_data.longitude, last_gps_data.altitude, last_gps_data.velocity, last_gps_data.satellites);
            break;
        
        default:
            printf("Unknown sensor type\n");
            break;
    }
};


