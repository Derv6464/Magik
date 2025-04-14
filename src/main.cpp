#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif
#include <hardware/flash.h>

#include "tools/state_machine.h"
#include "sensors/data.h"
#include "sensors/barometer.h"
#include "sensors/sensor_handler.h"
#include "outputs/status_led.h"
#include "tools/logger.h"
#include "tools/interfaces.h"
#include "sensors/config.h"

#include "sensors/drivers/test_input/test_handler.h"
#include "sensors/drivers/test_input/tester_core.h"
#include "sensors/drivers/test_input/tester_sec.h"

char *p = (char *)XIP_BASE;

//I2C bus for high g accelerometer
#define sda 0
#define scl 1

//SPI bus for barometer and low g accelerometer/IMU
#define sck_1 10
#define mosi_1 11
#define miso_1 12
#define cs_baro 13
#define cs_imu 5

//SPI bus for LoRa
#define sck_0 18
#define mosi_0 19
#define miso_0 16
#define cs_lora 17
#define rst_lora 20

#define neopixel 14
#define buzzer 15

//UART bus for GPS
//#define tx_1 8
//#define rx_1 9
#define rst_gps 7

#ifdef TESTING
//UART bus for testing
#define tx_1 8
#define rx_1 9

#endif

//Pyro channels
#define pyro_en 6
#define pyro_1 21
#define pyro_2 22
#define pyro_check_1 26
#define pyro_check_2 27

void handle_init() { printf("State: INIT\n"); }
void handle_bluetooth_settings() {
    //start bt settings server
    printf("State: BLUETOOTH_SETTINGS\n"); }
void handle_calibrating() { 
    //
    printf("State: CALIBRATING\n"); 
}
void handle_ready() { printf("State: READY\n"); }
void handle_powered() { printf("State: POWERED\n"); }
void handle_coasting() { printf("State: COASTING\n"); }
void handle_drouge() { printf("State: DROUGE\n"); }
void handle_main() { printf("State: MAIN\n"); }
void handle_landed() { printf("State: LANDED\n"); }

static void run_task(void* pvParameters) {
    printf("State Machine Task\n");
    StateMachine* fsm = static_cast<StateMachine*>(pvParameters);
    fsm->run(pvParameters);
}

void run_core_sensors(void* pvParameters) {
    printf("Core Sensor Task\n");
    SensorHandler<core_flight_data>* handler = static_cast<SensorHandler<core_flight_data>*>(pvParameters);
    handler->runSensors();  // This function contains an infinite loop
}

void run_test_hander(void* pvParameters) {
    TestHandler* handler = static_cast<TestHandler*>(pvParameters);
    while (true) {
        handler->split_data();
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}


void print_core_sensor_data(void* pvParameters) {
    QueueHandle_t coreDataQueue = static_cast<QueueHandle_t>(pvParameters);
    core_flight_data data;
    
    while (true) {
        if (xQueueReceive(coreDataQueue, &data, portMAX_DELAY) == pdTRUE) {
            printf("Time: %d, Pressure: %d\n", data.time, data.barometer.pressure);
            printf("Acceleration: X=%d, Y=%d, Z=%d\n", data.acceleration.x, data.acceleration.y, data.acceleration.z);
            printf("Temperature: %d°C, BT Active: %d\n", data.barometer.temperature, data.bt_active);
        } else {
            printf("Failed to receive data from queue\n");
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

static void printRunning(void* pvParameters){
    while (1){
        printf("Running\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
int main() {
    stdio_init_all();
    QueueHandle_t coreDataQueue = xQueueCreate(3, sizeof(core_flight_data));
    QueueHandle_t secDataQueue = xQueueCreate(3, sizeof(secondary_flight_data));
    QueueHandle_t stateQueue = xQueueCreate(1, sizeof(int));

    printf("Starting...\n");
    //SPI spi_0(sck_1, mosi_1, miso_1, spi0);
    uart_inst_t* port = uart_get_instance(1);
    UART uart_1(tx_1, rx_1, port, 115200, 11);
    #ifdef TESTING
    TestHandler testHandler(&uart_1);
    printf("TestHandler created\n");
    Barometer barometer(&testHandler); 
    #endif
    printf("UART created\n");
    
     // Barometer barometer(&uart_1, &name); 
    printf("snsor\n");
    Sensor<core_flight_data>* barometerSensor = barometer.getSensor();
    //IMU imu(&spi_0, cs_imu);
 
    
    //UART uart(tx_1, rx_1);
    //GPS gps(&uart, rst_gps);

    //I2C i2c(sda, scl);
    //Accel accel(&i2c);

    //SPI spi_1(sck_1, mosi_1, miso_1);
    //Radio lora(&spi_1, cs_lora, rst_lora);

    SensorHandler<core_flight_data> core_sensors(coreDataQueue);
    core_sensors.addSensor(barometerSensor);


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


    //init state machine
    //init logger
    //init status led
    //init sensores
    StateMachine fsm(state_handlers);
    //xTaskCreate(run_task, "Flight_State_Task", 256, &fsm, 1, NULL);
    //xTaskCreate(run_core_sensors, "CoreSensorTask", 256, &core_sensors, 1, NULL);
    //xTaskCreate(print_core_sensor_data, "PrintSensorTask", 256, coreDataQueue, 1, NULL);
    #ifdef TESTING
    xTaskCreate(run_test_hander, "TestHandlerTask", 256, &testHandler, 1, NULL);
    #endif
    //xTaskCreate(printRunning, "PrintTask", 256, NULL, 1, NULL);


    vTaskStartScheduler();

    while (1) {

       
    };

}
