#ifndef DRIVER_H_
#define DRIVER_H_

// Template base class
template <typename T>
class Driver {
public:
    virtual ~Driver() = default;
    virtual void update(T& data) = 0;  
};

#endif // DRIVER_H_
