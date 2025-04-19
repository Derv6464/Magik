#ifndef RADIO_H
#define RADIO_H
#include "../config.h"
#include "../tools/interfaces.h"
#include "../drivers/driver_out.h"

#ifdef TESTING
#include "../drivers/test_input/tester_radio.h"
#endif

class Radio{
public:
    Radio(SPI* spi);
    Radio();

    void init();
    void sendData(const char* data);
    //void receiveData(char* buffer, size_t length);

    private:
        DriverOut* radio;
        SPI* spi;

};

#endif // RADIO_H