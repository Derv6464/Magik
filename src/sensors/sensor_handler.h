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

    void runSensors() {
        T data = {};  

        while (true) {
            for (auto sensor : sensors_) {
                sensor->update(data);  // Each sensor updates the data struct
            }

            xQueueSend(data_queue_, &data, portMAX_DELAY);
            vTaskDelay(pdMS_TO_TICKS(100));  // Prevent excessive CPU usage
        }
    }

private:
    std::vector<Sensor<T>*> sensors_;
    QueueHandle_t data_queue_;
};

#endif // SENSOR_HANDLER_H_
