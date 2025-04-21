#ifndef LOGGER_H
#define LOGGER_H
#include "../config.h"
#include "../drivers/driver_out.h"

#ifdef LOG_INTERNAL_FLASH
#include "../drivers/flash_internal/flash_internal.h"
#endif

class Logger{
public:
    Logger();
    ~Logger();
    void send(int* data);
    void init();

private:
    DriverOut* logger = nullptr;
};

#endif // LOGGER_H