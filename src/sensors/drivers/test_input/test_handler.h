#include "../../config.h"
#ifdef TESTING
#ifndef TEST_HANDLER_H
#define TEST_HANDLER_H
#include "../../data.h"
#include "../../../tools/interfaces.h"
#include "FreeRTOS.h"

class TestHandler 
{
public:
    TestHandler(UART* serial);
    ~TestHandler();
    void split_data();

    baro_data last_baro_data;
    accle_data last_accle_data;
    gps_data last_gps_data;
private:
    UART* serial;
    
};



#endif // TEST_HANDLER_H
#endif