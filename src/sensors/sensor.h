#ifndef SENSOR_H_
#define SENSOR_H_

// Template base class
template <typename T>
class Sensor {
public:
    virtual ~Sensor() = default;
    virtual void update(T& data) = 0;  // Pure virtual function to update data of type T
};

#endif // SENSOR_H_
