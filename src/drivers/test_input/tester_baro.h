#include "../../config.h"
#ifdef TESTING
#ifndef TEST_BARO_H
#define TEST_BARO_H
#include "../driver_in.h"
#include "../../tools/interfaces.h"
#include "../../sensors/data.h"
#include <stdio.h>
#include "test_handler.h"

class Tester_Baro : public DriverIn<baro_data> {
    public:
        Tester_Baro(TestHandler* handler);
        ~Tester_Baro() override { printf("Tester destroyed\n"); }
        void update(baro_data& data) override;
    private:
        TestHandler* hander;
};

#endif // TEST_BARO_H
#endif