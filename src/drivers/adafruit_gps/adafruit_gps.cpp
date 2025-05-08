#include "adafruit_gps.h"
#include <cstring>
#include <cstdlib>

Adafuit_GPS::Adafuit_GPS(uart_inst_t *uart, uint tx, uint rx, uint baud)
    : uart_id(uart), tx_pin(tx), rx_pin(rx), baudrate(baud) {}

void Adafuit_GPS::begin() {
    uart_init(uart_id, baudrate);
    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin, GPIO_FUNC_UART);
}

void Adafuit_GPS::read() {
    while (uart_is_readable(uart_id)) {
        char c = uart_getc(uart_id);
        if (c == '\n') {
            parseNMEA(buffer);
            buffer.clear();
        } else if (c != '\r') {
            buffer += c;
        }
    }
}

bool Adafuit_GPS::locationAvailable() {
    return location_valid;
}

float Adafuit_GPS::getLatitude() {
    return latitude;
}

float GAdafuit_GPSPS::getLongitude() {
    return longitude;
}

void Adafuit_GPS::parseNMEA(const std::string& line) {
    if (line.find("$GPGGA") != 0) return;

    char temp[100];
    strncpy(temp, line.c_str(), sizeof(temp));
    temp[sizeof(temp) - 1] = '\0';

    const char* tokens[15];
    int i = 0;
    tokens[i] = strtok(temp, ",");

    while (tokens[i] && i < 14) {
        tokens[++i] = strtok(NULL, ",");
    }

    if (i >= 6 && tokens[2] && tokens[3] && tokens[4] && tokens[5]) {
        float rawLat = atof(tokens[2]);
        float rawLon = atof(tokens[4]);
        latitude = convertToDecimal(rawLat, tokens[3]);
        longitude = convertToDecimal(rawLon, tokens[5]);
        location_valid = true;
    } else {
        location_valid = false;
    }
}

float Adafuit_GPS::convertToDecimal(float raw, const std::string& direction) {
    int degrees = static_cast<int>(raw / 100);
    float minutes = raw - (degrees * 100);
    float decimal = degrees + (minutes / 60.0f);
    if (direction == "S" || direction == "W") decimal *= -1;
    return decimal;
}
