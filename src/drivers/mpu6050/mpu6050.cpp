#include "../../config.h"
#ifdef ACCEL_MPU6050
#include "mpu6050.h"

MPU6050::MPU6050(I2C_BASE *i2c, int addr){
    this->i2c = i2c;
    this->device_addr = addr;
    reset();

    if (!get_id()){
        printf("MPU6050 not found\n");
    }
    else{
        printf("MPU6050 found\n");
    }

    update_range();
    set_range(MPU6050_RANGE_2_G);
};

MPU6050::~MPU6050(){
    printf("MPU6050 destroyed\n");
};

void MPU6050::update(accle_data& data){
    int16_t accel[3];
    read_raw_accel(accel);

    float accel_scale = 1.0;
    switch (accel_range) {
        case MPU6050_RANGE_2_G:  accel_scale = 16384.0; break;
        case MPU6050_RANGE_4_G:  accel_scale = 8192.0; break;
        case MPU6050_RANGE_8_G:  accel_scale = 4096.0; break;
        case MPU6050_RANGE_16_G: accel_scale = 2048.0; break;
    }

    data.x = ((float)accel[0])/ accel_scale;
    data.y = ((float)accel[1])/ accel_scale;
    data.z = ((float)accel[2])/ accel_scale;
};

void MPU6050::reset(){
    uint8_t buf[] = {0x6B, 0x80};
    i2c->write(device_addr, buf, 2);

    //cahnge cause no freeRTOS riunnign for debug
    //vTaskDelay(pdMS_TO_TICKS(100));

    buf[1] = 0x00; 
    i2c->write(device_addr, buf, 2);
    //vTaskDelay(pdMS_TO_TICKS(10)); 
}

void MPU6050::read_raw_accel(int16_t accel[3]){
    uint8_t buffer[6];
    i2c->read(device_addr, MPU6050_ACCEL_OUT, buffer, 6);

    for (int i = 0; i < 3; i++) {
        accel[i] = (int16_t)((buffer[i * 2] << 8) | buffer[i * 2 + 1]);
    }
}


bool MPU6050::get_id(){
    uint8_t buffer[1];
    i2c->read(device_addr, MPU6050_ID_LOC, buffer, 1);
    printf("MPU6050 ID: %02x\n", buffer[0]);
    if (buffer[0] == MPU6050_ID){
        return true;
    }
    return false;
}

void MPU6050::set_range(mpu6050_accel_range_t accel_range_in){
    uint8_t buf[2];
    buf[0] = MPU6050_ACCEL_CONFIG;
    buf[1] = (accel_range_in << 3);
    i2c->write(device_addr, buf, 2);
    printf("MPU6050 set range: %d\n", accel_range_in);
    update_range();
}

void MPU6050::update_range(){
    uint8_t buf[1];
    i2c->read(device_addr, MPU6050_ACCEL_OUT, buf, 1);
    accel_range = (mpu6050_accel_range_t)((buf[0] >> 3) & 0x03);
    printf("MPU6050 range: %d\n", accel_range);
}
#endif