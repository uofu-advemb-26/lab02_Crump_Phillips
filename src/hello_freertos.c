/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"



#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define BLINK_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2UL )
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

void blink_task(void *params) {
    hard_assert(cyw43_arch_init() == PICO_OK);

    int count = *(int *)params[0];
    bool on = *(bool *)params[1];  
    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
        if (count++ % 11) on = !on; // flip 10 outta 11 times
        vTaskDelay(500); // 500 ms
    }
}

void main_task(void *params) {
    xTaskCreate(blink_task, "BlinkThread",
                BLINK_TASK_STACK_SIZE, &params, BLINK_TASK_PRIORITY, NULL);
    char c;
    while(c = getchar()) {
        if (c <= 'z' && c >= 'a') putchar(c - 32);
        else if (c >= 'A' && c <= 'Z') putchar(c + 32);
        else putchar(c);
    }
}


int main( void )
{
    stdio_init_all();

    static int count = 0;
    static bool on = false;
    void *params[2] = {&count, &on};

    const char *rtos_name;
    rtos_name = "FreeRTOS";
    TaskHandle_t task;
    xTaskCreate(main_task, "MainThread",
                MAIN_TASK_STACK_SIZE, &params, MAIN_TASK_PRIORITY, &task);
    vTaskStartScheduler();
    return 0;
}
