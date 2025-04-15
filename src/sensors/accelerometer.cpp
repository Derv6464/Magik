#include "accelerometer.h"

Accelerometer::Accelerometer(SPI *spi, int cs) {
    printf("Accelerometer created, spi\n");
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(cs);
    gpio_set_dir(cs, GPIO_OUT);
    gpio_put(cs, 1);
    // Make the CS pin available to picotool
    bi_decl(bi_1pin_with_name(cs, "SPI CS ACCEL"));

    #ifdef ACCEL_MPU6050
        printf("Accelerometer MPU6050\n");
        //MPU6050 accel(spi, cs);
        //accelerometer = &accel;
    #endif

}

Accelerometer::Accelerometer(I2C *i2c, int addr) {
    printf("Accelerometer created, i2c\n");

    #ifdef ACCEL_ADXL345
        printf("Accelerometer ADXL345\n");
        //ADXL345 accel(i2c, addr);
        //accelerometer = &accel;
    #endif

}

Accelerometer::Accelerometer(TestHandler* handler){
    #ifdef TESTING
        printf("Accelerometer Tester\n");
        accelerometer = new Tester_Core(handler, 'a');
    #endif
    printf("Accelerometer created\n");
}

Accelerometer::~Accelerometer(){
    if (accelerometer != nullptr) {
        delete accelerometer;
        accelerometer = nullptr;
    }
    printf("Accelerometer destroyed\n");
}

