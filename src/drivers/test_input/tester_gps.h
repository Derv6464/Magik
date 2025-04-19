#include "../../config.h"
#ifdef TESTING
#ifndef TEST_INPUT_S_H
#define TEST_INPUT_S_H
#include "../driver.h"
#include "../../tools/interfaces.h"
#include "../../sensors/data.h"
#include <stdio.h>
#include "test_handler.h"

class Tester_Gps : public Driver<gps_data> {
    public:
        Tester_Gps(TestHandler* handler);
        ~Tester_Gps() override { printf("Tester destroyed\n"); }
        void update(gps_data& data) override;
    private:
        TestHandler* hander;
};



#endif // TEST_INPUT_S_H
#endif