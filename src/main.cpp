#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif
#include <hardware/flash.h>

char *p = (char *)XIP_BASE;

#define LED_PIN 14


int main()
{
    stdio_init_all();

    //init state machine
    //init logger
    //init status led
    //init sensores

    //begin state machine task
    //begin status led task
    while(1){};
}