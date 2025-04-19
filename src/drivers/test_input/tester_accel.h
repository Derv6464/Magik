#include "../../config.h"
#ifdef TESTING
#ifndef TEST_ACCEL_H
#define TEST_ACCEL_H
#include "../driver.h"
#include "../../tools/interfaces.h"
#include "../../sensors/data.h"
#include <stdio.h>
#include "test_handler.h"

class Tester_Accel: public Driver<accle_data> {
    public:
        Tester_Accel(TestHandler* handler);
        ~Tester_Accel() override { printf("Tester destroyed\n"); }
        void update(accle_data& data) override;
    private:
        TestHandler* hander;
};

#endif // TEST_ACCEL_H
#endif