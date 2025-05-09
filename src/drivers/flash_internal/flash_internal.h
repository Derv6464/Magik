#include "../../config.h"
#ifdef LOG_INTERNAL_FLASH
#ifndef FLASH_LOGGER_H
#define FLASH_LOGGER_H
#include "../driver_out.h"
#include <stdio.h>
#include "pico/flash.h"
#include "hardware/flash.h"
#include "../../sensors/data.h"

class FlashInternal : public DriverOut {
public:
    FlashInternal();
    void send(int* data) override;
    void save_settings(flash_internal_data settings);
    flash_internal_data get_settings();
private:
    static void call_flash_range_erase(void *param);
    static void call_flash_range_program(void *param);
    char temp_storage[4000];
    int temp_fill_level{0};
    char overflow[10];
    int sector{LOG_FLASH_OFFSET};
};



#endif // FLASH_LOGGER_H
#endif