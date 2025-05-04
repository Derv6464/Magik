#ifndef PYRO_H
#define PYRO_H
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>

class Pyro{
    public:
        Pyro(int pin_1, int pin_2, int pin_en);
        void unlock();
        void lock();
        void fire(int pyro_num);

    
    private:
        int pyro_1_pin;
        int pyro_2_pin;
        int pyro_en_pin;

        bool locked;
};

#endif // PYRO_H