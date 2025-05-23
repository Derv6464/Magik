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
    reg |= READ_BIT;
    cs_select(cs);
    spi_write_blocking(port, &reg, 1);
    //sleep_ms(10); doesnt work in debug
    printf("bla bla bla\n");  //killin time cause debug hates sleep
    spi_read_blocking(port, 0, buf, len);
    cs_deselect(cs);
    //sleep_ms(10); doesnt work in debug
    printf("bla bla bla\n");  //killin time cause debug hates sleep
}

void SPI::write(int cs, uint8_t reg, uint8_t *buf, uint16_t len) {
    reg |= READ_BIT;
    cs_select(cs);
    spi_write_blocking(port, &reg, 1);
    //sleep_ms(10); doesnt work in debug
    printf("bla bla bla\n");  //killin time cause debug hates sleep
    cs_deselect(cs);
    //sleep_ms(10); doesnt work in debug
    printf("bla bla bla\n");  //killin time cause debug hates sleep
}

void SPI::read_no_cs(uint8_t reg, uint8_t *buf, uint16_t len) {
    printf("read_no_cs reg: %02x\n", reg);
    reg |= READ_BIT;
    // Perform full-duplex SPI transaction
    spi_write_blocking(port, &reg, 1);
    spi_read_blocking(port, 0, buf, len);

    printf("read_no_cs buf: ");
    for (int i = 0; i < len; i++) {
        printf("%02x ", buf[i]);
    }

    printf("read_no_cs done\n");
}

void SPI::write_no_cs(uint8_t* reg, uint8_t const *buf, uint16_t len) {
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.

    //reg |= READ_BIT;
    spi_write_blocking(port, reg, len);
    //sleep_ms(10); doesnt work in debug
    printf("bla bla bla\n");  //killin time cause debug hates sleep
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

I2C::I2C(int sda, int scl, i2c_inst_t* port) {
    this->sda = sda;
    this->scl = scl;
    this->port = port;
    setup();
};

void I2C::setup() {
    i2c_init(port, 100 * 1000);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(scl);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(sda, scl, GPIO_FUNC_I2C));
};

void I2C::write(int addr, uint8_t* data, uint len){
    i2c_write_blocking(i2c_default, addr, data, len, false); 
    //sleep_ms(10); doesnt work in debug
    printf("bla bla bla\n");  //killin time cause debug hates sleep

};

void I2C::read(int addr, uint8_t data, uint8_t* buf, uint len) {
    i2c_write_blocking(i2c_default, addr, &data, 1, true);
    i2c_read_blocking(i2c_default, addr, buf, len, false);
    printf("\n");  //killin time cause debug hates sleep
};

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



I2C_PIO::I2C_PIO(int sda, int scl, PIO pio){
    this->sda = sda;
    this->scl = scl;
    this->pio = pio;

    setup();
}
void I2C_PIO::write(int addr, uint8_t* data, uint len){
    pio_i2c_write_blocking(pio, sm, addr, data, len);
    //sleep_ms(10); doesnt work in debug
    printf("bla bla bla\n");  //killin time cause debug hates sleep
    
}
void I2C_PIO::read(int addr, uint8_t data, uint8_t* buf, uint len){
    pio_i2c_read_blocking(pio, sm, addr, buf, len);

}

void I2C_PIO::setup(){
    uint offset = pio_add_program(pio, &i2c_program);
    i2c_program_init(pio, sm, offset, sda, scl);
}
