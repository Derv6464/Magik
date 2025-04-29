file(GLOB SOURCES
    main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/outputs/*.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sensors/*.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/drivers/bmp390/*.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/drivers/bmp390/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/MPU6050/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/test_input/*.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/flash_internal/*.cpp
)
# Debugging: Print the collected sources
message(STATUS "Collected sources: ${SOURCES}")

add_executable(${ProjectName} ${SOURCES})

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${EIGEN_PATH}      
)

set_target_properties(${ProjectName} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(${ProjectName}  pico_cyw43_arch_none)
endif()

target_link_libraries(${ProjectName} 
    pico_stdlib 
    FreeRTOS-Kernel-Heap4 
    hardware_pio
    hardware_flash 
    hardware_sync
    hardware_spi
    hardware_i2c
    pico_btstack_ble
    pico_btstack_cyw43
    pico_cyw43_arch_none
    hardware_adc
    )

#pico_btstack_make_gatt_header(${ProjectName}  PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tools/bt_server_info.gatt")

pico_add_extra_outputs(${ProjectName})

pico_generate_pio_header(${ProjectName} ${CMAKE_CURRENT_SOURCE_DIR}/outputs/ws2812.pio)