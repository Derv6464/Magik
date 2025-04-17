#ifndef PROCESSING_H
#define PROCESSING_H

float const sea_level_pressure = 1013.25F; // Sea level pressure in Pascals

float getAltitude(float pressure, float temperature);
float getVelocity(float last_velocity, float accel, float time);

#endif // PROCESSING_H