add_executable(${ProjectName}
    main.c
)

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
    )

pico_add_extra_outputs(${ProjectName})