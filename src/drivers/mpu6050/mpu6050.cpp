#include "mpu6050.h"

MPU6050::MPU6050(I2C *i2c, int addr){
    this->i2c = i2c;
    this->addr = addr;
    reset();
};

MPU6050::~MPU6050(){
    printf("MPU6050 destroyed\n");
};

void MPU6050::update(accle_data& data){
    int16_t accel[3];
    read_accel(accel);

    data.x = accel[0];
    data.y = accel[1];
    data.z = accel[2];

    printf("accel: %d %d %d\n", data.x, data.y, data.z);
};

void MPU6050::reset(){
    uint8_t buf[] = {0x6B, 0x80};
    i2c->write(addr, buf, 2);

    //cahnge cause no freeRTOS riunnign for debug
    vTaskDelay(pdMS_TO_TICKS(100)); // Allow device to reset and stabilize

    // Clear sleep mode (0x6B register, 0x00 value)
    buf[1] = 0x00; 
    i2c->write(addr, buf, 2); // Clear sleep mode by writing 0x00 to the 0x6B register
    vTaskDelay(pdMS_TO_TICKS(10)); // Allow stabilization after waking up
}

void MPU6050::read_accel(int16_t accel[3]){

    uint8_t buffer[6];

    // Start reading acceleration registers from register 0x3B for 6 bytes
    uint8_t val = 0x3B;
    i2c->read(addr, val, buffer, 6);


    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

}