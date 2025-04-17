#include "processing.h"
#include <cmath>

float getHeight(float pressure) {
    // Formula from BMP180 datasheet, given temp = 15C and sea level pressure = 1013.25hPa
    return 44330.0 * (1.0 - pow(pressure / sea_level_pressure, 0.1903));
}

float getVelocity(float last_velocity, float accel, float time) {
    // v = u + at
    return last_velocity + (accel * time); 
    
}