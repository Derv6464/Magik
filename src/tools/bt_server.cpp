//#include "bt_server.h"
//
//bt_server::bt_server() {
//    printf("Bluetooth server created\n");
//    cyw43_arch_init();
//    btstack_cyw43_init();
//    btstack_cyw43_set_power(1);
//    btstack_cyw43_set_mode(BTSTACK_CYW43_MODE_BLE);
//    btstack_cyw43_set_power(0);
//}
//
//bt_server::~bt_server() {
//    printf("Bluetooth server destroyed\n");
//    btstack_cyw43_set_power(0);
//    btstack_cyw43_deinit();
//}