add_library(lab02_functions STATIC
    functions.c
)
# This is the main binary. List your C files here.
add_executable(lab02
    main.c
    )

target_include_directories(lab02 PRIVATE ${PROJECT_SOURCE_DIR}/include)

pico_set_program_name(lab02 "lab02")
pico_set_program_version(lab02 "0.1")

# Modify the below lines to enable/disable output over UART/USB.
# These are mutually exclusive, only set one.
pico_enable_stdio_uart(lab02 0)
pico_enable_stdio_usb(lab02 1)

# Add the libraries to the build
target_link_libraries(lab02  PRIVATE
  lab02_functions
  pico_stdlib
  pico_multicore
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
)

# The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# variable will be set and the wireless libraries added.
if(PICO_CYW43_SUPPORTED)
    # For led support on pico_w
    target_link_libraries(lab02 PRIVATE
        pico_cyw43_arch_none
    )
endif()

# Another pico macro which generates the uf2 file in addition to the elf
pico_add_extra_outputs(lab02)

find_program(OPENOCD openocd)
find_program(PICOTOOL picotool)

add_custom_target(flash COMMAND ${PICOTOOL} load -f $<TARGET_FILE:lab02> DEPENDS lab02)
add_custom_target(openocd_flash ${OPENOCD} -s ${OPENOCD_PATH} -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c 'adapter speed 5000' -c program "$<TARGET_FILE:lab02>" verify reset exit' DEPENDS lab02)
