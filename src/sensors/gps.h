#ifndef GPS_H
#define GPS_H

#include "sensor.h"
#include "drivers/driver.h"
#include "tools/interfaces.h"
#include "data.h"
#include "config.h"
#include <stdio.h>

#ifdef TESTING
#include "drivers/test_input/tester_gps.h"
#include "drivers/test_input/test_handler.h"
#endif

class GPS: public Sensor<secondary_flight_data> {
    public:
        GPS(UART *uart);
        GPS(TestHandler* handler);
        ~GPS();
        void update(secondary_flight_data& data) override;
    private:
        Driver<gps_data>* gps;
};

#endif // GPS_H