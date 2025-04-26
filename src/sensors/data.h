#ifndef DATA_H
#define DATA_H
#include "FreeRTOS.h"
#include "queue.h"

struct accle_data
{
    float x{0};
    float y{0};
    float z{0};
};

struct gyro_data
{
    int x{0};
    int y{0};
    int z{0};
};

struct baro_data
{
    int pressure{101325};
    float temperature{0};
    float altitude{0};
};

struct prediction_data
{
    float position{0};
    float velocity{0};
    float acceleration{0};
};

struct core_flight_data
{
    int time{0};
    baro_data barometer;
    accle_data acceleration;
    prediction_data prediction;
    float velocity{0};
    bool setting_pin;
    bool bt_active;
};

struct gps_data
{
    double latitude;
    double longitude;
    float altitude;
    float velocity;
    int satellites;
};

struct imu_data
{
    accle_data acceleration;
    gyro_data gyro;
    int temperature;
};

struct secondary_flight_data
{
    int time;
    gps_data gps;
    accle_data acceleration;
    imu_data imu;
    float velocity{0};
};

struct flight_data
{
    core_flight_data core_data;
    secondary_flight_data secondary_data;
    int state;
};


struct AllQueuesArgs {
    QueueHandle_t coreDataQueue;
    QueueHandle_t secDataQueue;
    QueueHandle_t flightDataQueue;
};

#endif // DATA_H

