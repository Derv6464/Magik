#include "accelerometer.h"

Accelerometer::Accelerometer(SPI *spi, int cs) {
    printf("Accelerometer created, spi\n");
    gpio_init(cs);
    gpio_set_dir(cs, GPIO_OUT);
    gpio_put(cs, 1);
    bi_decl(bi_1pin_with_name(cs, "SPI CS ACCEL"));

    #ifdef ACCEL_LSM6DSOX
        printf("Accelerometer LSM6DSOX\n");
        accelerometer = new LSM6DSOX(spi, cs);
    #endif

}

Accelerometer::Accelerometer(I2C *i2c, int addr) {
    printf("Accelerometer created, i2c\n");

    #ifdef ACCEL_ADXL345
        printf("Accelerometer ADXL345\n");
        ADXL345 accelerometer = new ADXL345(i2c, addr);
    #endif
    #ifdef ACCEL_MPU6050
        printf("Accelerometer MPU6050\n");
        accelerometer = new MPU6050(i2c, addr);
    #endif

}

#ifdef TESTING
Accelerometer::Accelerometer(TestHandler* handler){
    
        printf("Accelerometer Tester\n");
        accelerometer = new Tester_Accel(handler);
    
    printf("Accelerometer created\n");
}
#endif

Accelerometer::~Accelerometer(){
    if (accelerometer != nullptr) {
        delete accelerometer;
        accelerometer = nullptr;
    }
    printf("Accelerometer destroyed\n");
}

void Accelerometer::update(core_flight_data* data){
    //printf("velocity: %f\n", data->velocity);
    accle_data accel_data;
    accelerometer->update(accel_data);

    float currentTime = to_ms_since_boot(get_absolute_time());
    
    data->acceleration.x = accel_data.x * -1.0f; // Invert x axis
    data->acceleration.y = accel_data.y;
    data->acceleration.z = accel_data.z;

    //printf("Accel: %f %f %f\n", data->acceleration.x, data->acceleration.y, data->acceleration.z);
    //data->velocity = getVelocity(data->velocity, accel_data.z, data->time);
    data->time = currentTime;
}

float Accelerometer::getVelocity(float last_velocity, float accel, float old_time) {
    float currentTime = to_ms_since_boot (get_absolute_time());
    //float abs_accel = sqrtf((accel * accel));
    // v = u + at
    return last_velocity + ((accel*-1) * ((currentTime - old_time)/1000)); 
}