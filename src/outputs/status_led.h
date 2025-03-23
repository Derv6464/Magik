#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

class Status_led{
public:
    /** Constructor
    *
    * @param pin pin number of the status led
    */
    Status_led(int pin);

    /** Destructor
    */
    ~Status_led();

    enum Color : uint32_t {
        RED = 0x00FF00,
        GREEN  = 0xFF0000,
        BLUE = 0x0000FF,
        YELLOW = 0xFFFF00,
        PURPLE = 0xFF00FF,
        CYAN = 0x00FFFF,
        WHITE = 0xFFFFFF,
        OFF = 0x000000
    };
    /** Set the color of the status led
     * 
     * @param color color of the status led
     * @return void
     * 
    */
    void set_color(Color color);
 
    /** Blink the status led
     * 
     * @param color color of the status led
     * @param duration duration of the blink
     * @return void
     * 
    */
    void blink(Color color, int duration);

    /** Clear the status led
     * 
     * @return void
     * 
    */
    void clear();
    

    private:
        PIO pio;
        uint sm;
        uint offset;

        void put_pixel(uint32_t pixel_grb);

       
};

