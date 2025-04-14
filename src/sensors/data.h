#ifndef DATA_H
#define DATA_H

struct accle_data
{
    int x;
    int y;
    int z;
};

struct gyro_data
{
    int x;
    int y;
    int z;
};

struct baro_data
{
    int pressure;
    float temperature;
};

struct core_flight_data
{
    int time;
    baro_data barometer;
    accle_data acceleration;
    bool setting_pin;
    bool bt_active;
};

struct gps_data
{
    float latitude;
    float longitude;
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
    gps_data gps;
    accle_data acceleration;
    imu_data imu;
};

struct flight_data
{
    core_flight_data core_data;
    secondary_flight_data secondary_data;
    int state;
};

#endif // DATA_H

