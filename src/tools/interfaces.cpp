#include "interfaces.h"

SPI::SPI(int sck, int mosi, int miso, spi_inst_t* port) {
    this->sck = sck;
    this->mosi = mosi;
    this->miso = miso;
    this->port = port;

    setup();
}

void SPI::setup() {
    spi_init(port, 1 * 1000 * 1000);
    gpio_set_function(sck, GPIO_FUNC_SPI);
    gpio_set_function(mosi, GPIO_FUNC_SPI);
    gpio_set_function(miso, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    bi_decl(bi_3pins_with_func(miso, mosi, sck, GPIO_FUNC_SPI));
}

void SPI::read(int cs, uint8_t reg, uint8_t *buf, uint16_t len) {
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.

    reg |= READ_BIT;
    cs_select(cs);
    spi_write_blocking(port, &reg, 1);
    sleep_ms(10);
    spi_read_blocking(port, 0, buf, len);
    cs_deselect(cs);
    sleep_ms(10);
}

void SPI::write(int cs, uint8_t reg, uint8_t *buf, uint16_t len) {
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.

    reg |= READ_BIT;
    cs_select(cs);
    spi_write_blocking(port, &reg, 1);
    sleep_ms(10);
    cs_deselect(cs);
    sleep_ms(10);
}

void SPI::read_no_cs(uint8_t reg, uint8_t *buf, uint16_t len) {
    

    printf("read_no_cs reg: %02x\n", reg);
    reg |= READ_BIT;
    printf("read_no_cs reg: %02x\n", reg);
    // Perform full-duplex SPI transaction
    spi_write_blocking(port, &reg, 1);
    spi_read_blocking(port, 0, buf, len);

    printf("read_no_cs done\n");
}

void SPI::write_no_cs(uint8_t* reg, uint8_t const *buf, uint16_t len) {
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.

    //reg |= READ_BIT;
    spi_write_blocking(port, reg, len);
    sleep_ms(10);
}


void SPI::cs_select(int cs){
    printf("cs_select %d\n", cs);
    asm volatile("nop \n nop \n nop");
    gpio_put(cs, 0);  // Active low
    asm volatile("nop \n nop \n nop");
    printf("cs_select done\n");
}

void SPI::cs_deselect(int cs){
    asm volatile("nop \n nop \n nop");
    gpio_put(cs, 1);
    asm volatile("nop \n nop \n nop");
}

void SPI_Device::read(uint8_t reg, uint8_t *buf, uint16_t len) {
    spi->read(cs, reg, buf, len);
}
void SPI_Device::write(uint8_t reg, uint8_t *buf, uint16_t len) {
    spi->write(cs, reg, buf, len);
}


UART::UART(int tx, int rx, uart_inst_t* port, int baudrate, int packet_size) {
    this->packet_size = packet_size;
    this->tx = tx;
    this->rx = rx;
    this->port = port;
    this->baudrate = baudrate;

    setup();
};

void UART::setup() {
    uart_init(port, baudrate);
    gpio_set_function(tx, UART_FUNCSEL_NUM(port, tx));
    gpio_set_function(rx, UART_FUNCSEL_NUM(port, rx));
};

void UART::write(char* data){
     uart_puts(port, data);
};

void UART::read(char* buffer){
    int bytesRead = 0;
    
    while (bytesRead < packet_size){
        if (uart_is_readable_within_us(port, 100)){
            buffer[bytesRead] = uart_getc(port);
            bytesRead++;
        }
    }
};

void UART::read(char* buffer, int custom_packet_size){
    int bytesRead = 0;
    
    while (bytesRead < custom_packet_size){
        if (uart_is_readable_within_us(port, 10)){
            buffer[bytesRead] = uart_getc(port);
            bytesRead++;
        }
    }
};