#ifndef INTERFACES_H
#define INTERFACES_H

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "i2c_pio/pio_i2c.h"

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

class I2C_BASE{
    public:
        virtual void write(int addr, uint8_t* data, uint len) = 0;
        virtual void read(int addr, uint8_t data, uint8_t* buf, uint len) = 0;
        virtual ~I2C_BASE() = default;
};

class I2C : public I2C_BASE {
    public:
        I2C(int sda, int scl, i2c_inst_t* port);
        void write(int addr, uint8_t* data, uint len) override;
        void read(int addr, uint8_t data, uint8_t* buf, uint len) override;

    private:
        void setup();
        i2c_inst_t* port;
        int sda;
        int scl;
};

class UART{
    public:
        UART(int tx, int rx, uart_inst_t* port, int baudrate, int packet_size);
        ~UART() { printf("UART destroyed\n"); }
        void write(char* data);
        void read(char* buffer);
        void read(char* buffer, int custom_packet_size);
        int getPacketSize() { return packet_size; }

    private:
        void setup();
        int tx;
        int rx;
        uart_inst_t* port;
        int baudrate;
        int packet_size;
};

class I2C_PIO: public I2C_BASE{
    public:
        I2C_PIO(int sda, int scl, PIO pio);
        void write(int addr, uint8_t* data, uint len) override;
        void read(int addr, uint8_t data, uint8_t* buf, uint len) override;

    private:
        void setup();
        PIO pio;
        int sda;
        int scl;
        uint sm{0};
};

#endif // INTERFACES_H