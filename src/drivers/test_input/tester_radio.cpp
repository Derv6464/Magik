#include "../../config.h"
#ifdef TESTING
#include "tester_radio.h"

Tester_Radio::Tester_Radio() {
    printf("Radio Tester created\n");
};

void Tester_Radio::send(int* data){
    printf("Radio Tester send\n");
    process_data(data);
    printf("faking sending: %d\n", *data);
};

void Tester_Radio::process_data(int* data) {
    printf("Fake processing data\n");
};

#endif
