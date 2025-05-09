#ifndef DRIVER_OUT_H_
#define DRIVER_OUT_H_

class DriverOut {
public:
    virtual ~DriverOut() = default;
    virtual void send(int* data) = 0;
};

#endif // DRIVER_OUT_H_
