#include "state_machine.h"
#include <stdio.h>

StateMachine::StateMachine(StateHandler handlers[]) {
    // Initialize the state machine with function pointer array
    for (int i = 0; i < NUM_STATES; ++i) {
        state_handlers[i] = handlers[i];
        called_once[i] = false;  // Mark that functions haven't been called
    }
    current_state = INIT;
}

void StateMachine::update_state(core_flight_data data){
    printf("State Machine\n");
    switch (current_state)
    {
    case INIT:
        printf("init\n");
        check_settings_state_done(data.setting_pin);
        break;
    case BLUETOOTH_SETTINGS:
        printf("bluetooth settings\n");
        check_bt_done(data.bt_active, data.setting_pin);
        break;
    case CALIBRATING:
        printf("calibrating\n");
        check_calibrating_state_done();
        break;
    case READY:
        printf("ready\n");
        check_ready_state_done(data.acceleration.x);
        break;
    case POWERED:
        printf("powered\n");
        check_powered_state_done(data.acceleration.x);
        break;
    case COASTING:
        printf("coasting\n");
        check_coasting_state_done(data.velocity);
        break;
    case DROUGE:
        printf("apogee, drouge\n");
        check_drouge_state_done(data.barometer.pressure);
        break;
    case MAIN:
        printf("main\n");
        check_drouge_state_done(data.barometer.pressure);
        break;
    case LANDED:
        printf("landed\n");
        break;
    default:
        break;
    }

    last_data = data;
    return;
}

void StateMachine::check_settings_state_done(bool setting_pin){
    if (setting_pin){
        change_state(State::BLUETOOTH_SETTINGS);
    }
    else{
        change_state(State::CALIBRATING);
    }
    return;
}

void StateMachine::check_bt_done(bool bt_active, bool setting_pin){
    if (!bt_active && !setting_pin){
        change_state(State::CALIBRATING);
    }
    return;
}

void StateMachine::check_calibrating_state_done(){
    printf("State Machine\n");
}

void StateMachine::check_ready_state_done(float accel_x){
    if (accel_x > 0.5f){
        change_state(State::POWERED);
    }
}

void StateMachine::check_powered_state_done(float accel_x){
    if (accel_x < 0.5f){
        change_state(State::COASTING);
    }
}

void StateMachine::check_coasting_state_done(float velocity){
    if (velocity < 0.0f){
        change_state(State::DROUGE);
    }
}

void StateMachine::check_drouge_state_done(float height){
    if (height < 100.0f){ // add main var
        change_state(State::MAIN);
    }
}

void StateMachine::check_main_state_done(float velocity){
    if (velocity < 0.0f){
        change_state(State::LANDED);
    }
}

void StateMachine::change_state(State new_state){
    if (new_state != current_state){
        if (!called_once[current_state]) {
            state_handlers[current_state](); //make sure this becomes async/ freeRTOS task 
            called_once[current_state] = true;  
        }
        current_state = new_state;
    }
}

void StateMachine::run(void * pvParameters ){
    //do init stuff
    while (true){
        //get data from sensors

        core_flight_data data;
        update_state(data);
        vTaskDelay(500);
    }
}

