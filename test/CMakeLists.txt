
function(configure_lab_test target_name)
    target_include_directories(${target_name} PRIVATE
        ${PROJECT_SOURCE_DIR}/include
    )

    target_link_libraries(${target_name} PRIVATE
        lab02_functions
        pico_async_context_freertos
        FreeRTOS-Kernel-Heap4
        pico_cyw43_arch_none
        pico_stdlib
        unity
    )

    pico_enable_stdio_uart(${target_name} 1)
    pico_enable_stdio_usb(${target_name} 0)

    pico_add_extra_outputs(${target_name})
endfunction()

# unit testing
add_executable(mytest
    test_functions.c
    unity_config.c
)

configure_lab_test(mytest)

# Hardware integration test
add_executable(hardware_test
    test_blink_hardware.c
    unity_config.c
)

configure_lab_test(hardware_test)

# The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# variable will be set and the wireless libraries added.
if(PICO_CYW43_SUPPORTED)
    target_link_libraries(mytest PRIVATE
        pico_cyw43_arch_none
    )
endif()

# Modify the below lines to enable/disable output over UART/USB.
# These are mutually exclusive, only set one.
pico_enable_stdio_uart(mytest 1)
pico_enable_stdio_usb(mytest 0)

# This will speed up the simulation, since data won't be copied from the flash controller.
# It does make the binary non-persistent on a real microcontroller.
pico_set_binary_type(mytest no_flash)

# Locate Renode
find_program(RENODE
    NAMES
        renode
        macos_run.command
        Renode.exe
    PATHS
        /Applications/Renode.app/Contents/MacOS
)

if(RENODE)
    set(RENODE_FLAGS
        --disable-gui
        --port
        -2
        --pid-file
        renode.pid
        --console
    )

    add_test(
        NAME simulate_mytest
        COMMAND
            ${RENODE}
            ${RENODE_FLAGS}
            -e "$ELF=@$<TARGET_FILE:mytest>; $WORKING=@${CMAKE_SOURCE_DIR}; include @${CMAKE_SOURCE_DIR}/simulate.resc; start"
    )

    set_tests_properties(simulate_mytest PROPERTIES
        TIMEOUT 30
    )
else()
    message(WARNING
        "Renode was not found"
    )
endif()

find_program(PICOTOOL picotool)

if(PICOTOOL)
    add_custom_target(flash_test
        COMMAND ${PICOTOOL} load -f $<TARGET_FILE:mytest>
        COMMAND ${PICOTOOL} reboot
        DEPENDS mytest
    )

    add_custom_target(flash_hardware_test
        COMMAND ${PICOTOOL} load -f $<TARGET_FILE:hardware_test>
        COMMAND ${PICOTOOL} reboot
        DEPENDS hardware_test
    )
endif()

# Locate OpenOCD
find_program(OPENOCD_EXECUTABLE
    NAMES openocd
)
# we can use picotools or openocd, openocd is required for the ci/cd workflow though.
if(OPENOCD_EXECUTABLE)
    set(OPENOCD_SCRIPTS_PATH "${OPENOCD_PATH}")

    if(EXISTS "${OPENOCD_PATH}/scripts")
        set(OPENOCD_SCRIPTS_PATH "${OPENOCD_PATH}/scripts")
    endif()

    add_custom_target(openocd_test
        COMMAND
            ${OPENOCD_EXECUTABLE}
            -s ${OPENOCD_SCRIPTS_PATH}
            -f interface/cmsis-dap.cfg
            -f target/rp2040.cfg
            -c "adapter speed 5000"
            -c "program $<TARGET_FILE:mytest> verify reset exit"
        DEPENDS mytest
        USES_TERMINAL
        COMMENT "Flashing mytest with OpenOCD"
    )

    add_custom_target(openocd_hardware_test
        COMMAND
            ${OPENOCD_EXECUTABLE}
            -s ${OPENOCD_SCRIPTS_PATH}
            -f interface/cmsis-dap.cfg
            -f target/rp2040.cfg
            -c "adapter speed 5000"
            -c "program $<TARGET_FILE:hardware_test> verify reset exit"
        DEPENDS hardware_test
        USES_TERMINAL
        COMMENT "Flashing hardware_test with OpenOCD"
    )
else()
    message(WARNING
        "OpenOCD was not found"
    )
endif()
