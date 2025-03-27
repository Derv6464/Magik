file(GLOB SOURCES
    main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/outputs/*.cpp  # This will include all .cpp files in the outputs directory
)

add_executable(${ProjectName} ${SOURCES})

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
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
    )

pico_add_extra_outputs(${ProjectName})

pico_generate_pio_header(${ProjectName} ${CMAKE_CURRENT_SOURCE_DIR}/outputs/ws2812.pio)