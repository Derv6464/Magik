#include "radio.h"
#include "logger.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "../sensors/data.h"

class Telemetry{
public:
    Telemetry(Radio* radio, Logger* logger);
    void serialise(flight_data* data, int serialised_data[45]);
    void send_data();
    void run(void* pvParameters);
private:
    Radio* radio;
    Logger* logger;
};