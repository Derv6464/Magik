#include "../sensor/data.h"

enum state_machine{
    INIT,
    BLUETOOTH_SETTINGS,
    CALIBRATING,
    READY,
    POWERED,
    COASTING,
    DROUGE,
    MAIN,
    LANDED
};

struct rocket_state
{
   state_machine state;
   flight_data data;

};
