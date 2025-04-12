#ifndef INTERFACES_H
#define INTERFACES_H

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "hardware/timer.h"

#define READ_BIT 0x80

class SPI{
    public:
        SPI(int sck, int mosi, int miso, spi_inst_t* port);
        void read(int cs, uint8_t reg, uint8_t *buf, uint16_t len);
        void read_no_cs(uint8_t reg, uint8_t *buf, uint16_t len);
        void write(int cs, uint8_t reg, uint8_t *buf, uint16_t len);
        void write_no_cs(uint8_t* reg, uint8_t const *buf, uint16_t len);
        void cs_select(int cs);
        void cs_deselect(int cs);

    private:
        void setup();
        int sck;
        int mosi;
        int miso;
        spi_inst_t* port;
};

class SPI_Device{
    public:
        SPI_Device(SPI* spi, int cs){
            this->spi = spi;
            this->cs = cs;
        };
        void read(uint8_t reg, uint8_t *buf, uint16_t len);
        void write(uint8_t reg, uint8_t *buf, uint16_t len);

    private:
        SPI* spi;
        int cs;
};

class I2C{
    public:
        I2C(int sda, int scl){
            this->sda = sda;
            this->scl = scl;
        };
        void write(int data);
        int read();

    private:
        int sda;
        int scl;
};

//class UART{
//
//}


class USB{
    public:
        USB(int tx, int rx, int port, int baudrate);
        void write(int data);
        int read();

    private:
        int tx;
        int rx;
}
#endif // INTERFACES_H