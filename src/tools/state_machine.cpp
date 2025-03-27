#include "state_machine.h"


namespace state_machine{
    void update_state(&rocket_state current_state){
        printf("State Machine\n");
        switch (current_state.state)
        {
        case INIT:
            if check_settings_state(current_state){
                current_state.state = BLUETOOTH_SETTINGS;
            }else{
                current_state.state = CALIBRATING;
            }
            break;
        case BLUETOOTH_SETTINGS:
            printf("calibrating\n");
            break;
        case :
            printf("ready\n");
            break;
        case 3:
            printf("powered\n");
            break;
        case 4:
            printf("coasting\n");
            break;
        case 5:
            printf("apogee, drouge\n");
            break;
        case 6:
            printf("main\n");
            break;
        case 7:
            printf("landed\n");
            break;
        }
    };
    bool check_settings_state(rocket_state current_state){
        if (current_state.state == INIT && current_state.data.setting_pin){ 
            return true;
        }else{
            return false;
        }
    };
    bool check_calibrating_state();
    bool check_ready_state();
    bool check_powered_state();
    bool check_coasting_state();
    bool check_drouge_state();
    bool check_main_state();
    bool check_landed_state(); 
};

void run(){
    state_machine rocket_state = INIT;
    //do init stuff
    while (true){

        //check state & update
        printf("State Machine\n");
        //wait
    }
}
