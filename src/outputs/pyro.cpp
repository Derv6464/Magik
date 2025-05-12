#include "pyro.h"


Pyro::Pyro(int pin_1, int pin_2, int pin_en) {
    pyro_1_pin = pin_1;
    pyro_2_pin = pin_2;
    pyro_en_pin = pin_en;

    locked = true;

    gpio_init(pyro_1_pin);
    gpio_set_dir(pyro_1_pin, GPIO_OUT);
    gpio_put(pyro_1_pin, 0);

    gpio_init(pyro_2_pin);
    gpio_set_dir(pyro_2_pin, GPIO_OUT);
    gpio_put(pyro_2_pin, 0);

    gpio_init(pyro_en_pin);
    gpio_set_dir(pyro_en_pin, GPIO_OUT);
    gpio_put(pyro_en_pin, 0);
}

void Pyro::unlock() {
    gpio_put(pyro_en_pin, 1);
    locked = false;
}
void Pyro::lock() {
    gpio_put(pyro_en_pin, 0);
    locked = true;
}

void Pyro::fire(int pyro_num) {
    if (locked) {
        printf("Pyro is locked, cannot fire\n");
        return;
    }
    if (pyro_num == 1) {
        gpio_put(pyro_1_pin, 1);
        busy_wait_ms(500);
        gpio_put(pyro_1_pin, 0);
    } else if (pyro_num == 2) {
        gpio_put(pyro_2_pin, 1);
        busy_wait_ms(500);
        gpio_put(pyro_2_pin, 0);
    } else {
        printf("Invalid pyro number\n");
    }
}