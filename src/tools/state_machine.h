#include "../sensors/data.h"
#include "FreeRTOS.h"
#include "task.h" 
#include "queue.h"
#include "../config.h"
#include "kalman_filter.h"
#include <stdio.h>
#include "pico/stdlib.h"

class StateMachine {
public:
    using StateHandler = std::function<void()>;
    enum State {
        CALIBRATING,
        READY,
        POWERED,
        COASTING,
        DROUGE,
        MAIN,
        LANDED,
        NUM_STATES
    };

    StateMachine(StateHandler handlers[], flash_internal_data settings);
    void run(void * pvParameters );

    KalmanFilter kalman_filter;
private:
    StateHandler state_handlers[NUM_STATES];
    bool called_once[NUM_STATES];
    State current_state;
    
    void update_state(core_flight_data raw_data, prediction_data prediction);
    void check_settings_state_done(bool setting_pin);
    void check_bt_done(bool bt_active, bool setting_pin);
    void check_calibrating_state_done();
    void check_ready_state_done(float accel);
    void check_powered_state_done(float accel);
    void check_coasting_state_done(float velocity);
    void check_drouge_state_done(float height);
    void check_main_state_done(float height);
    void change_state(State new_state);

    //hard coded low limit values in case settings are not set
    int main_height{200};
    int drouge_delay{0};
    int liftoff_threshold{20};
};
