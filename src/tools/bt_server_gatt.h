/**
 * Copyright (c) 2023 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 #ifndef SERVER_COMMON_H_
 #define SERVER_COMMON_H_
 
 #include "btstack_defines.h"
#include "ble/att_db.h"
#include "ble/att_server.h"
#include "btstack_util.h"
#include "bluetooth_gatt.h"
#include "btstack_debug.h"

#include "bt_server_info.h"

 typedef struct {

	// Connection handle for service
	hci_con_handle_t con_handle ;

	// get apogee information
	char * 		get_apogee_value;
	char * 		get_apogee_description ;
	uint16_t  	get_apogee_handle ;
	uint16_t 	get_apogee_description_handle ;

    // set apogee information
    char * 		set_apogee_value;
    char * 		set_apogee_description ;
    uint16_t 	set_apogee_handle ;
    uint16_t 	set_apogee_description_handle ;

    //get main information
    char * 		get_main_value;
    char * 		get_main_description ;
    uint16_t 	get_main_handle ;
    uint16_t 	get_main_description_handle ;

    //set main information
    char * 		set_main_value;
    char * 		set_main_description ;
    uint16_t 	set_main_handle ;
    uint16_t 	set_main_description_handle ;

    //set done
    char * 		set_done_value;
    char * 		set_done_description ;
    uint16_t 	set_done_handle ;
    uint16_t 	set_done_description_handle ;

} custom_service_t ;

static att_service_handler_t 	service_handler ;
static custom_service_t 		service_object ;

char get_apogee[] = "get_apogee";
char get_main[] = "get_main";
char set_apogee[] = "set_apogee";
char set_main[] = "set_main";
char set_done[] = "set_done";

static uint16_t custom_service_read_callback(hci_con_handle_t con_handle, uint16_t attribute_handle, uint16_t offset, uint8_t * buffer, uint16_t buffer_size){
    printf("Read handle: 0x%04x\n", attribute_handle);
	UNUSED(con_handle);
    printf("Read callback\n");
	if (attribute_handle == service_object.get_apogee_handle){
        printf("Get apogee value: %s\n", service_object.get_apogee_value);
		return att_read_callback_handle_blob((const uint8_t *)service_object.get_apogee_value, strlen(service_object.get_apogee_value), offset, buffer, buffer_size);
	}
	if (attribute_handle == service_object.get_apogee_description_handle) {
        printf("Get apogee description: %s\n", service_object.get_apogee_description);
		return att_read_callback_handle_blob((const uint8_t *)service_object.get_apogee_description, strlen(service_object.get_apogee_description), offset, buffer, buffer_size);
	}

	if (attribute_handle == service_object.get_main_handle){
        printf("Get main value: %s\n", service_object.get_main_value);
        return att_read_callback_handle_blob((const uint8_t *)service_object.get_main_value, strlen(service_object.get_main_value), offset, buffer, buffer_size);
    }
    if (attribute_handle == service_object.get_main_description_handle) {
        printf("Get main description: %s\n", service_object.get_main_description);
        return att_read_callback_handle_blob((const uint8_t *)service_object.get_main_description, strlen(service_object.get_main_description), offset, buffer, buffer_size);
    }

    return 0;
}

static int custom_service_write_callback(hci_con_handle_t con_handle, uint16_t attribute_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size){
    printf("Read handle: 0x%04x\n", attribute_handle);
    UNUSED(transaction_mode);
	UNUSED(offset);
	UNUSED(buffer_size);
    printf("Write callback\n");
    if (attribute_handle == service_object.set_apogee_handle) 
    {
        custom_service_t * instance = &service_object ;
        // Read the apogee value
        memcpy(buffer, service_object.set_apogee_value, strlen(service_object.set_apogee_value));
        buffer[strlen(service_object.set_apogee_value)] = 0;
        printf("Set apogee value: %s\n", service_object.set_apogee_value);
        return strlen(service_object.set_apogee_value);
    }
    else if (attribute_handle == service_object.set_main_handle) 
    {
        custom_service_t * instance = &service_object ;
        // Read the main value
        printf("Set main value: %s\n", service_object.set_main_value);
        memcpy(buffer, service_object.set_main_value, strlen(service_object.set_main_value));
        buffer[strlen(service_object.set_main_value)] = 0;
        return strlen(service_object.set_main_value);
    }
    else if (attribute_handle == service_object.set_done_handle) 
    {
        custom_service_t * instance = &service_object ;
        // Read the done value
        printf("Set done value: %s\n", service_object.set_done_value);
        memcpy(buffer, service_object.set_done_value, strlen(service_object.set_done_value));
        buffer[strlen(service_object.set_done_value)] = 0;
        if (strcmp(service_object.set_done_value, "done") == 0) {
            // Set the done value to 0
            service_object.set_done_value[0] = 0;
            //rtos notify
        }
        return strlen(service_object.set_done_value);
    }

    return 0;
}

att_service_handler_t custom_service_server_init(char * get_apogee_ptr, char * get_main_ptr, char * set_apogee_ptr, char * set_main_ptr, char * set_done_ptr){


	// Pointer to our service object
	custom_service_t * instance = &service_object ;

    // Assign characteristic value
	instance->get_apogee_value = get_apogee_ptr ;
    instance->get_main_value = get_main_ptr ;
    instance->set_apogee_value = set_apogee_ptr ;
    instance->set_main_value = set_main_ptr ;
    instance->set_done_value = set_done_ptr ;

    // Assign characteristic user description
	instance->get_apogee_description = get_apogee;
    instance->get_main_description = get_main;
    instance->set_apogee_description = set_apogee;
    instance->set_main_description = set_main;
    instance->set_done_description = set_done;


    instance->get_apogee_handle = ATT_CHARACTERISTIC_CB8BDA85_220B_4A2C_A350_721F10261851_01_VALUE_HANDLE ;
	instance->get_apogee_description_handle = ATT_CHARACTERISTIC_CB8BDA85_220B_4A2C_A350_721F10261851_01_USER_DESCRIPTION_HANDLE ;

    instance->set_apogee_handle = ATT_CHARACTERISTIC_CB8BDA85_220B_4A2C_A350_721F10261852_01_VALUE_HANDLE ;
    instance->set_apogee_description_handle = ATT_CHARACTERISTIC_CB8BDA85_220B_4A2C_A350_721F10261852_01_USER_DESCRIPTION_HANDLE ;

    instance->get_main_handle = ATT_CHARACTERISTIC_CB8BDA85_220B_4A2C_A350_721F10261853_01_VALUE_HANDLE ;
    instance->get_main_description_handle = ATT_CHARACTERISTIC_CB8BDA85_220B_4A2C_A350_721F10261853_01_USER_DESCRIPTION_HANDLE ;

    instance->set_main_handle = ATT_CHARACTERISTIC_CB8BDA85_220B_4A2C_A350_721F10261854_01_VALUE_HANDLE ;
    instance->set_main_description_handle = ATT_CHARACTERISTIC_CB8BDA85_220B_4A2C_A350_721F10261854_01_USER_DESCRIPTION_HANDLE ;

    instance->set_done_handle = ATT_CHARACTERISTIC_CB8BDA85_220B_4A2C_A350_721F10261855_01_VALUE_HANDLE ;
    instance->set_done_description_handle = ATT_CHARACTERISTIC_CB8BDA85_220B_4A2C_A350_721F10261855_01_USER_DESCRIPTION_HANDLE ;

	// Service start and end handles (modeled off heartrate example)
	service_handler.start_handle = 0 ;
	service_handler.end_handle = 0xFFFF ;
	service_handler.read_callback = &custom_service_read_callback ;
	service_handler.write_callback = &custom_service_write_callback ;

    printf("Custom service handler initialized\n");

	// Register the service handler
    return service_handler;
}


 #endif