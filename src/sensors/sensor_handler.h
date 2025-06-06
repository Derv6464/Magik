#ifndef SENSOR_HANDLER_H_
#define SENSOR_HANDLER_H_

#include <vector>
#include "sensor.h"
#include "FreeRTOS.h"
#include "queue.h"

template <typename T>
class SensorHandler {
public:
    explicit SensorHandler(QueueHandle_t data_queue) : data_queue_(data_queue) {}

    ~SensorHandler() {
        for (auto sensor : sensors_) {
            delete sensor;
        }
    }

    void addSensor(Sensor<T>* sensor) {
        sensors_.push_back(sensor);
    }

    void runSensors(T* data) {
        while (true) {
            //printf("SensorHandler: Running\n");
            
            for (auto sensor : sensors_) {
                sensor->update(data); 
            }

            xQueueSend(data_queue_, data, pdMS_TO_TICKS(10));
            data->time = to_ms_since_boot(get_absolute_time());
            vTaskDelay(pdMS_TO_TICKS(read_data_delay)); 
            
        }
    }

private:
    std::vector<Sensor<T>*> sensors_;
    QueueHandle_t data_queue_;
};

#endif // SENSOR_HANDLER_H_
