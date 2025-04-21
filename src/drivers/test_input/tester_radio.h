#include "../../config.h"
#ifdef TESTING
#ifndef TEST_RADIO_H
#define TEST_RADIO_H
#include "../driver_out.h"
#include "../../sensors/data.h"
#include <stdio.h>

class Tester_Radio : public DriverOut {
    public:
        Tester_Radio();
        ~Tester_Radio() { printf("Radio Tester destroyed\n"); }
        void send(int* data) ;
    private:
        void process_data(int* data);
};

#endif // TEST_RADIO_H
#endif