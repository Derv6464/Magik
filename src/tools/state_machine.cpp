#include "state_machine.h"
#include <stdio.h>

StateMachine::StateMachine(StateHandler handlers[]) {
    // Initialize the state machine with function pointer array
    for (int i = 0; i < NUM_STATES; ++i) {
        state_handlers[i] = handlers[i];
        called_once[i] = false;  // Mark that functions haven't been called
    }
    current_state = State::INIT;
}

void StateMachine::update_state(core_flight_data data){
    //printf("State Machine\n");
    printf("State: %d\n", current_state);
    //printf("Data: %d %d %f %f %f\n", data.time, data.barometer.pressure, data.acceleration.x, data.velocity, data.setting_pin);
    switch (current_state)
    {
    case State::INIT:
        printf("init\n");
        check_settings_state_done(data.setting_pin);
        break;
    case State::BLUETOOTH_SETTINGS:
        printf("bluetooth settings\n");
        check_bt_done(data.bt_active, data.setting_pin);
        break;
    case State::CALIBRATING:
        printf("calibrating\n");
        check_calibrating_state_done();
        break;
    case State::READY:
        printf("ready\n");
        check_ready_state_done(data.acceleration.x, data.acceleration.y, data.acceleration.z);
        break;
    case State::POWERED:
        printf("powered\n");
        check_powered_state_done(data.acceleration.x, data.acceleration.y, data.acceleration.z);
        break;
    case State::COASTING:
        printf("coasting\n");
        check_coasting_state_done(data.velocity);
        break;
    case State::DROUGE:
        printf("apogee, drouge\n");
        check_drouge_state_done(data.barometer.altitude);
        break;
    case State::MAIN:
        printf("main\n");
        check_drouge_state_done(data.barometer.altitude);
        break;
    case State::LANDED:
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
    change_state(State::READY);
    return;
}

void StateMachine::check_ready_state_done(float accel_x, float accel_y, float accel_z){
    float accel = (accel_x * accel_x) + (accel_y * accel_y) + (accel_z * accel_z);
    float threshold = 10;
    printf("State Machine: Ready, accel: %f, threshold: %f", accel, threshold);
    if (accel > threshold){
        change_state(State::POWERED);
    }
}

void StateMachine::check_powered_state_done(float accel_x, float accel_y, float accel_z){
    float accel = (accel_x * accel_x) + (accel_y * accel_y) + (accel_z * accel_z);
    if (accel_x < 0.5f){
        printf("State Machine: Coasting, accel_x: %f\n", accel_x);
        change_state(State::COASTING);
    }
}

void StateMachine::check_coasting_state_done(float velocity){
    if (velocity < 0.0f){
        printf("State Machine: Drouge, velocity: %f\n", velocity);
        change_state(State::DROUGE);
    }
}

void StateMachine::check_drouge_state_done(float height){
    if (height < 100.0f){ // add main var
        printf("State Machine: Main, height: %f\n", height);
        change_state(State::MAIN);
    }
}

void StateMachine::check_main_state_done(float velocity){
    if (velocity < 0.0f){
        printf("State Machine: Landed, velocity: %f\n", velocity);
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
    //printf("State Machine Task\n");
    QueueHandle_t coreDataQueue = static_cast<QueueHandle_t>(pvParameters);
    while (true){

        core_flight_data data;
        if (xQueueReceive(coreDataQueue, &data, portMAX_DELAY) == pdTRUE) {
            update_state(data);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

