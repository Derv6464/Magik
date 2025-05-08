#ifndef GPS_H
#define GPS_H

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <string>

class Adafuit_GPS {
public:
    Adafuit_GPS(uart_inst_t *uart, uint tx_pin, uint rx_pin, uint baudrate = 9600);
    void begin();
    void read();
    bool locationAvailable();
    float getLatitude();
    float getLongitude();

private:
    uart_inst_t *uart_id;
    uint tx_pin, rx_pin;
    uint baudrate;
    std::string buffer;

    float latitude = 0.0;
    float longitude = 0.0;
    bool location_valid = false;

    void parseNMEA(const std::string& line);
    float convertToDecimal(float raw, const std::string& direction);
};

#endif
