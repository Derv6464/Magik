#ifndef GPS_H
#define GPS_H

#include "sensor.h"
#include "tools/interfaces.h"
#include "data.h"
#include "config.h"
#include <stdio.h>

#ifdef TESTING
#include "drivers/test_input/tester_sec.h"
#include "drivers/test_input/test_handler.h"
#endif

class GPS{
    public:
        GPS(UART *uart);
        GPS(TestHandler* handler);
        ~GPS();
        Sensor<secondary_flight_data>* getSensor() { return gps; }
    private:
        Sensor<secondary_flight_data>* gps = nullptr;
};

#endif // GPS_H