#include "../../config.h"
#ifdef TESTING
#ifndef TEST_INPUT_H
#define TEST_INPUT_H
#include "../../sensor.h"
#include "../../../tools/interfaces.h"
#include "../../data.h"
#include <stdio.h>
#include "test_handler.h"

class Tester_Core : public Sensor<core_flight_data> {
    public:
        Tester_Core(TestHandler* handler, char name);
        ~Tester_Core() override { printf("Tester destroyed\n"); }
        void update(core_flight_data& data) override;
    private:
        char name;
        TestHandler* hander;
};

#endif // TEST_INPUT_H
#endif