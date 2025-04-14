#include "../../config.h"
#ifdef TESTING
#ifndef TEST_INPUT_S_H
#define TEST_INPUT_S_H
#include "../../sensor.h"
#include "../../../tools/interfaces.h"
#include "../../data.h"
#include <stdio.h>
#include "test_handler.h"

class Tester_Sec : public Sensor<secondary_flight_data> {
    public:
        Tester_Sec(TestHandler* handler, char name);
        ~Tester_Sec() override { printf("Tester destroyed\n"); }
        void update(secondary_flight_data& data) override;
    private:
        char name;
        TestHandler* hander;
};



#endif // TEST_INPUT_S_H
#endif