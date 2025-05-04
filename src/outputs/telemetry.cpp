#include "telemetry.h"

Telemetry::Telemetry(Radio* radio_in, Logger* logger_in){
    this->radio = radio_in;
    this->logger = logger_in;
};

void Telemetry::run(void* pvParameters) {
    QueueHandle_t flightDataQueue = static_cast<QueueHandle_t>(pvParameters);
    //redo this
    flight_data data;
    int serialised_data[128];
    printf("Telemetry initalised\n");
    while (true) {
        if ((xQueueReceive(flightDataQueue, &data, pdMS_TO_TICKS(10)) == pdTRUE)) {

            //printf("GPS Latitude: %lf, Longitude: %lf\n", data.secondary_data.gps.latitude, data.secondary_data.gps.longitude);
            //printf("Time: %d, Pressure: %d\n", data.core_data.time, data.core_data.barometer.pressure);
            //printf("Acceleration: X=%f, Y=%f, Z=%f\n", data.core_data.acceleration.x, data.core_data.acceleration.y, data.core_data.acceleration.z);
            //printf("Temperature: %f°C, BT Active: %d\n", data.core_data.barometer.temperature, data.core_data.bt_active);
            //printf("\n");
            //printf("State: %d\n", data.state);
            serialise(&data, serialised_data);
            logger->send(serialised_data);
        }else{
            printf("Failed to receive data from flight queue\n");
        }
        vTaskDelay(pdMS_TO_TICKS(send_data_delay));
    }
};

void Telemetry::serialise(flight_data* data, int serialised_data[45]){
    //format of:
    //state|core_time|pressure|temp|altitude|velocity|accel|sec_time|lat|long|
    size_t offset = 0;
    memcpy(serialised_data + offset, &data->state, sizeof(data->state)); offset += sizeof(data->state);
    printf("state: %d, %d\n", data->state, serialised_data[0]);
    memcpy(serialised_data + offset, &data->core_data.time, sizeof(data->core_data.time)); offset += sizeof(data->core_data.time);
    memcpy(serialised_data + offset, &data->core_data.barometer.pressure, sizeof(data->core_data.barometer.pressure)); offset += sizeof(data->core_data.barometer.pressure);
    memcpy(serialised_data + offset, &data->core_data.barometer.temperature, sizeof(data->core_data.barometer.temperature)); offset += sizeof(data->core_data.barometer.temperature);
    memcpy(serialised_data + offset, &data->core_data.barometer.altitude, sizeof(data->core_data.barometer.altitude)); offset += sizeof(data->core_data.barometer.altitude);
    memcpy(serialised_data + offset, &data->core_data.acceleration, sizeof(data->core_data.acceleration)); offset += sizeof(data->core_data.acceleration);
    memcpy(serialised_data + offset, &data->secondary_data.time, sizeof(data->secondary_data.time)); offset += sizeof(data->secondary_data.time);
    // memcpy(serialised_data + offset, &data->secondary_data.gps.latitude, sizeof(data->secondary_data.gps.latitude)); offset += sizeof(data->secondary_data.gps.latitude);
    //memcpy(serialised_data + offset, &data->secondary_data.gps.longitude, sizeof(data->secondary_data.gps.longitude));
}