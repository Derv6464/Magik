#include "status_led.h"

Status_led::Status_led(int pin){
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &sm, &offset, pin, 1, true);
    hard_assert(success);

    ws2812_program_init(pio, sm, offset, pin, 800000, false);
};

Status_led::~Status_led(){
    pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);
};

void Status_led::set_color(Color color){
    put_pixel(color);
};

void Status_led::blink(Color color, int duration){
    put_pixel(color);
    //vTaskDelay(pdMS_TO_TICKS(500));
    put_pixel(OFF);
};

void Status_led::clear(){
    put_pixel(OFF);
};


void Status_led::put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
};

