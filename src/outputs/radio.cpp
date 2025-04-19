#include "radio.h"

Radio::Radio(SPI *spi) {
    printf("Radio created\n");
}

Radio::Radio(){
    #ifdef TESTING
        printf("Radio Tester\n");
        radio = new Tester_Radio();
    #endif
    printf("Radio created\n");
}

