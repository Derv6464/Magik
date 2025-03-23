#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#define LED_PIN 14

#include "outputs/status_led.h"

void led_task(void *pvParameters)
{
    int led_pin = LED_PIN;
    printf("LED Task\n");
    Status_led neopixel(led_pin);
    while(1)
    {
        printf("color red\n");
        neopixel.set_color(Status_led::RED);
        vTaskDelay(pdMS_TO_TICKS(500));
        printf("color green\n");
        neopixel.set_color(Status_led::GREEN);
        vTaskDelay(pdMS_TO_TICKS(500));
        printf("color blue\n");
        neopixel.set_color(Status_led::BLUE);
        vTaskDelay(pdMS_TO_TICKS(500));
        neopixel.clear();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


int main()
{
    stdio_init_all();

    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1){};
}