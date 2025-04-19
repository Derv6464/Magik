#include "../sensors/data.h"
#include "logger.h"
#include "FreeRTOS.h"
#include "task.h" 
#include "queue.h"

class StateMachine {
public:
    typedef void (*StateHandler)();
    enum State {
        INIT,
        BLUETOOTH_SETTINGS,
        CALIBRATING,
        READY,
        POWERED,
        COASTING,
        DROUGE,
        MAIN,
        LANDED,
        NUM_STATES
    };

    StateMachine(StateHandler handlers[]);
    void run(void * pvParameters );

private:
    StateHandler state_handlers[NUM_STATES];
    bool called_once[NUM_STATES];
    State current_state;
    
    core_flight_data last_data;
    void update_state(core_flight_data data);
    void check_settings_state_done(bool setting_pin);
    void check_bt_done(bool bt_active, bool setting_pin);
    void check_calibrating_state_done();
    void check_ready_state_done(float accel_x, float accel_y, float accel_z);
    void check_powered_state_done(float accel_x, float accel_y, float accel_z);
    void check_coasting_state_done(float velocity);
    void check_drouge_state_done(float height);
    void check_main_state_done(float height);
    void change_state(State new_state);
};
