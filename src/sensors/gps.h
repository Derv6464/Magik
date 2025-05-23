#ifndef GPS_H
#define GPS_H

#include "sensor.h"
#include "../drivers/driver_in.h"
#include "tools/interfaces.h"
#include "data.h"
#include "../config.h"
#include <stdio.h>

#ifdef TESTING
#include "../drivers/test_input/tester_gps.h"
#include "../drivers/test_input/test_handler.h"
#endif

class GPS: public Sensor<secondary_flight_data> {
    public:
        GPS(UART *uart);
        #ifdef TESTING
        GPS(TestHandler* handler);
        #endif
        ~GPS();
        void update(secondary_flight_data* data) override;
    private:
        DriverIn<gps_data>* gps;
};

#endif // GPS_H