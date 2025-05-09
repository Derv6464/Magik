#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif
#include <hardware/flash.h>

#include "btstack.h"
#include "pico/cyw43_arch.h"
#include "pico/btstack_cyw43.h"

#include "tools/bt_server_gap.h"
#include "tools/bt_server_gatt.h"
#include "tools/state_machine.h"
#include "tools/interfaces.h"
#include "tools/settings.h"

#include "sensors/barometer.h"
#include "sensors/accelerometer.h"
#include "sensors/gps.h"
#include "sensors/sensor_handler.h"
#include "sensors/data.h"

#include "config.h"

#include "outputs/status_led.h"
#include "outputs/radio.h"
#include "outputs/logger.h"
#include "outputs/telemetry.h"
#include "outputs/pyro.h"

#ifdef TESTING
#include "drivers/test_input/test_handler.h"
#endif

#include <Eigen/Dense>
 
using Eigen::MatrixXd;

void handle_init() { printf("State: INIT\n"); }
void handle_bluetooth_settings() {
    //start bt settings server
    printf("State: BLUETOOTH_SETTINGS\n"); }
void handle_calibrating() { 
    //
    printf("State: CALIBRATING\n"); 
}
void handle_ready() { printf("State: READY\n"); }
void handle_powered() { 
    printf("State: POWERED\n");
    printf("unlcking pyro\n");
 }
void handle_coasting() { printf("State: COASTING\n"); }
void handle_drouge() { 
    //ignite.fire(1);
    printf("State: DROUGE\n");
 }
void handle_main() { printf("State: MAIN\n"); }
void handle_landed() { printf("State: LANDED\n"); }

struct StateMachineArgs {
    StateMachine* fsm;
    AllQueuesArgs* queues; 
};

struct TelemetryArgs{
    Telemetry* telem;
    QueueHandle_t flightDataQueue;
};

static void run_task(void* pvParameters) {
    printf("State Machine Task\n");
    StateMachineArgs* args = static_cast<StateMachineArgs*>(pvParameters);
    args->fsm->run(args->queues);
};

static void run_telem(void* pvParameters) {
    printf("State Machine Task\n");
    TelemetryArgs* args = static_cast<TelemetryArgs*>(pvParameters);
    args->telem->run(args->flightDataQueue);
};

void run_core_sensors(void* pvParameters) {
    printf("Core Sensor Task\n");
    SensorHandler<core_flight_data>* handler = static_cast<SensorHandler<core_flight_data>*>(pvParameters);
    core_flight_data data;
    data.time = to_ms_since_boot(get_absolute_time());
    handler->runSensors(&data); 
};

void run_secondary_sensors(void* pvParameters) {
    printf("Secondary Sensor Task\n");
    SensorHandler<secondary_flight_data>* handler = static_cast<SensorHandler<secondary_flight_data>*>(pvParameters);
    secondary_flight_data data;
    handler->runSensors(&data); 
};


#ifdef TESTING
void run_test_hander(void* pvParameters) {
    TestHandler* handler = static_cast<TestHandler*>(pvParameters);
    while (true) {
        handler->split_data();
        vTaskDelay(pdMS_TO_TICKS(read_data_delay));
    }
};
#endif

//void print_core_sensor_data(void* pvParameters) {
//    QueueHandle_t coreDataQueue = static_cast<QueueHandle_t>(pvParameters);
//    core_flight_data data;
//    
//    while (true) {
//        if (xQueuePeek(coreDataQueue, &data, portMAX_DELAY) == pdTRUE) {
//            printf("Time: %d, Pressure: %d\n", data.time, data.barometer.pressure);
//            printf("Acceleration: X=%f, Y=%f, Z=%f\n", data.acceleration.x, data.acceleration.y, data.acceleration.z);
//            printf("Temperature: %f°C, BT Active: %d\n", data.barometer.temperature, data.bt_active);
//            printf("\n");
//        } else {
//            printf("Failed to receive data from core queue\n");
//        }
//        vTaskDelay(pdMS_TO_TICKS(1000));
//    }
//}

//void print_secondary_sensor_data(void* pvParameters) {
//    QueueHandle_t secDataQueue = static_cast<QueueHandle_t>(pvParameters);
//    secondary_flight_data data;
//    
//    while (true) {
//        if (xQueuePeek(secDataQueue, &data, portMAX_DELAY) == pdTRUE) {
//            printf("GPS Latitude: %lf, Longitude: %lf\n", data.gps.latitude, data.gps.longitude);
//            printf("\n");
//        } else {
//            printf("Failed to receive data from secondary queue\n");
//        }
//        vTaskDelay(pdMS_TO_TICKS(1000));
//    }
//}


//static void printRunning(void* pvParameters){
//    while (1){
//        printf("Running\n");
//        vTaskDelay(pdMS_TO_TICKS(1000));
//    }
//}
void run_flight(Status_led* status_led, FlightSettings* flight_settings) {
    QueueHandle_t coreDataQueue = xQueueCreate(3, sizeof(core_flight_data));
    QueueHandle_t secDataQueue = xQueueCreate(3, sizeof(secondary_flight_data));
    QueueHandle_t flightDataQueue = xQueueCreate(1, sizeof(flight_data));

    printf("Starting...\n");
    printf("flightDataQueue: %d\n", sizeof(flight_data));
    printf("coreDataQueue: %d\n", sizeof(core_flight_data));
    printf("secDataQueue: %d\n", sizeof(secondary_flight_data));

    Pyro ignite = Pyro(pyro_en, pyro_1, pyro_2);
    ignite.lock();
    
    uart_inst_t* port = uart_get_instance(1);
    i2c_inst_t* i2c_port = i2c_get_instance(0);

    #ifdef TESTING
    UART uart_1(tx_1, rx_1, port, 115200, 11);
    TestHandler testHandler(&uart_1);
  
    Barometer barometer(&testHandler); 
    Accelerometer accelerometer(&testHandler);
    GPS gps(&testHandler);
    Radio* radio = new Radio();
    Logger* logger = new Logger();
    #else
    SPI spi_1(sck_1, mosi_1, miso_1, spi1);
    Barometer barometer(&spi_1, cs_baro);
    //I2C i2c_0(sda_0, scl_0, i2c_port);
    //int mpu = 0x68;
    //Accelerometer accelerometer(&i2c_0, mpu);
    #endif

    printf("UART created\n");
    
    SensorHandler<core_flight_data> core_sensors(coreDataQueue);
    core_sensors.addSensor(&barometer);
    core_sensors.addSensor(&accelerometer);

    SensorHandler<secondary_flight_data> sec_sensors(secDataQueue);
    sec_sensors.addSensor(&gps);

    StateMachine::StateHandler state_handlers[] = {
        handle_init,
        handle_bluetooth_settings,
        handle_calibrating,
        handle_ready,
        handle_powered,
        handle_coasting,
        handle_drouge,
        handle_main,
        handle_landed
    };

    StateMachine* fsm = new StateMachine(state_handlers);
    //take in flight settings to init heights

    StateMachineArgs* fsmArgs = new StateMachineArgs{
        .fsm = fsm,
        .queues = new AllQueuesArgs{
            .coreDataQueue = coreDataQueue,
            .secDataQueue = secDataQueue,
            .flightDataQueue = flightDataQueue
        }
    };

    //Telemetry* telemetry = new Telemetry(radio, logger);
//
    //TelemetryArgs* telemArgs = new TelemetryArgs{
    //    .telem = telemetry,
    //    .flightDataQueue = flightDataQueue
    //};
 
    xTaskCreate(run_task, "Flight_State_Task", 512, fsmArgs, 3, NULL);
    xTaskCreate(run_core_sensors, "CoreSensorTask", 512, &core_sensors, 4, NULL);
    xTaskCreate(run_secondary_sensors, "SecondarySensorTask", 256, &sec_sensors, 2, NULL);
    #ifdef TESTING
    xTaskCreate(run_test_hander, "TestHandlerTask", 512, &testHandler, 5, NULL);
    #endif
    //xTaskCreate(printRunning, "PrintTask", 256, NULL, 3, NULL);
    //xTaskCreate(run_telem, "Telemetry Task", 512, telemArgs, 2, NULL);

    #ifdef TESTING
    gpio_init(neopixel);
    gpio_set_dir(neopixel, GPIO_OUT);

    // Pull the RUN pin low to reset the second Pico
    gpio_put(neopixel, 1);
    printf("RUN pin low\n");

    gpio_put(neopixel, 0);
    #endif

    vTaskStartScheduler();

    while (1) {
        //never reach here
    };
}

#define HEARTBEAT_PERIOD_MS 250

// BTstack objects
static btstack_timer_source_t heartbeat;
static btstack_packet_callback_registration_t hci_event_callback_registration;
static int characteristic_appogee_value = 0;

static char characteristic_apogee_value_tx[10];
static char characteristic_apogee_value_rx[10];
static char characteristic_main_value_tx[10];
static char characteristic_main_value_rx[10];
static char characteristic_done_value_rx[10];


int run_settings(Status_led* status_led, FlightSettings* flight_settings) {
    printf("Settings\n");
    status_led->set_color(Status_led::Color::BLUE);
     // initialize CYW43 driver architecture (will enable BT if/because CYW43_ENABLE_BLUETOOTH == 1)
     if (cyw43_arch_init()) {
        printf("failed to initialise cyw43_arch\n");
        return -1;
    }

    // Initialize L2CAP and security manager
    l2cap_init();
    sm_init();

    // Initialize ATT server, no general read/write callbacks
    // because we'll set one up for each service
    att_server_init(profile_data, NULL, NULL);   

    // Instantiate our custom service handler
    service_handler = custom_service_server_init( 
        characteristic_apogee_value_tx,
        characteristic_main_value_tx,
        characteristic_apogee_value_rx,
        characteristic_main_value_rx,
        characteristic_done_value_rx
    ); ;

    att_server_register_service_handler(&service_handler);
    printf("Registered service handler: start=0x%04x, end=0x%04x\n", service_handler.start_handle, service_handler.end_handle);
    printf("Registered service handler: start=0x%04x, end=0x%04x\n", service_handler.write_callback, service_handler.read_callback);

    // inform about BTstack state
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    // register for ATT event
    att_server_register_packet_handler(packet_handler);

    // turn on bluetooth!
    hci_power_control(HCI_POWER_ON);
    
    //if recveice notifiy
    //if (strcmp(characteristic_done_value_rx, "done") == 0) {
    //    printf("Received done\n");
    flash_internal_data settings = {
        .main_height = 1,
        .drouge_delay = 1,
        .liftoff_thresh = 1,
        .last_log = 0
    };  

    flight_settings->save_settings(settings);


    // btstack_run_loop_execute is only required when using the 'polling' method (e.g. using pico_cyw43_arch_poll library).
    // This example uses the 'threadsafe background` method, where BT work is handled in a low priority IRQ, so it
    // is fine to call bt_stack_run_loop_execute() but equally you can continue executing user code.

    #if 0 // btstack_run_loop_execute() is not required, so lets not use it
        btstack_run_loop_execute();
    #else
        // this core is free to do it's own stuff except when using 'polling' method (in which case you should use 
        // btstacK_run_loop_ methods to add work to the run loop.

        // this is a forever loop in place of where user code would go.
        while(true) {      
            //sleep_ms(1000);

            //printf("Running\n");
        }
    #endif
        return 0;
}

int main() {
    stdio_init_all();
    
    gpio_init(bt_setting_pin);
    gpio_set_dir(bt_setting_pin, GPIO_IN);
    gpio_pull_up(bt_setting_pin);

    Status_led status_led(neopixel);
    status_led.set_color(Status_led::Color::RED);
  
    FlightSettings flight_settings;
    flight_settings.get_settings();

    if (gpio_get(bt_setting_pin)) {  
        printf("Running settings\n");
        run_settings(&status_led, &flight_settings);
    } else {
        printf("Running flight\n");
        run_flight(&status_led, &flight_settings);
    }
};
