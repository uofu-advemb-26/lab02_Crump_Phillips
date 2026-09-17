/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * Edited by Matthew Crump and Will Phillips
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

void toggle(int* count, bool* on) {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, *on);
    bool retval = *count % 11;
    if (retval) *on = !(*on); // flip 10 outta 11 times
    *count++;
}

void blink_task(void *params) {
    hard_assert(cyw43_arch_init() == PICO_OK);

    //int count = *(int *)params[0];
    //bool on = *(bool *)params[1];  
		int count = 0;
		bool on = false;
    while (true) {
        toggle(&count, &on);
        vTaskDelay(500); // 500 ms
    }
}

char convert(char c) {
    if (c <= 'z' && c >= 'a') return c - 32;
    else if (c >= 'A' && c <= 'Z') return c + 32;
    else return c;
}

void main_task(void *params) {
    xTaskCreate(blink_task, "BlinkThread",
                BLINK_TASK_STACK_SIZE, &params, BLINK_TASK_PRIORITY, NULL);
    char c;
    while(c = getchar()){
         putchar(convert(c));
    }
}



int main( void )
{
    stdio_init_all();

//    static int count = 0;
//    static bool on = false;
//    void *params[2] = {&count, &on};

    const char *rtos_name;
    rtos_name = "FreeRTOS";
    TaskHandle_t task;
    xTaskCreate(main_task, "MainThread",
                MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &task);
    vTaskStartScheduler();
    return 0;
}
