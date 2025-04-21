#ifndef DRIVER_IN_H_
#define DRIVER_IN_H_

// Template base class
template <typename T>
class DriverIn {
public:
    virtual ~DriverIn() = default;
    virtual void update(T& data) = 0;  
};

#endif // DRIVER_IN_H_
