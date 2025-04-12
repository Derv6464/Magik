#include "../../config.h"
#ifdef TESTING
#ifndef TEST_INPUT_H
#define TEST_INPUT_H
#include "../../sensor.h"
#include "../../../tools/interfaces.h"
#include "../../data.h"
#include <stdio.h>

class Tester : public Sensor<secondary_flight_data> {
    public:
        Tester(Serial *serial);
        void update(secondary_flight_data& data) override;
    private:
        Serial *serial
        void read();
};


#endif // TEST_INPUT_H
#endif