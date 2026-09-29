/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 * Edited by Matthew Crump and William Phillips
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "functions.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#define MAIN_TASK_PRIORITY (tskIDLE_PRIORITY + 1U)
#define BLINK_TASK_PRIORITY (tskIDLE_PRIORITY + 2U)
#define MAIN_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 2U)
#define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define BLINK_DELAY_MS 500U

/** Task to control the Pico W LED. */
static void blink_task(void *parameters)
{
    (void)parameters;

    blink_state_t state;
    blink_state_init(&state);

    for (;;) {
        const bool output_level = blink_step(&state);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, output_level);
        vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY_MS));
    }
}

/** Create the LED task, then swap the case of each received serial byte. */
static void main_task(void *parameters)
{
    (void)parameters;

    const BaseType_t result = xTaskCreate(
        blink_task,
        "Blink",
        BLINK_TASK_STACK_SIZE,
        NULL,
        BLINK_TASK_PRIORITY,
        NULL
    );

    if (result != pdPASS) {
        printf("unable to create the blink task.\n");
        vTaskDelete(NULL);
        return;
    }

    for (;;) {
        const int input = getchar();

        if (input != EOF) {
            putchar(toggle_ascii_case(input));
        } else {
            taskYIELD(); // instead of busy waiting we can yield to other tasks if no input is available
        }
    }
}

int main(void)
{
    stdio_init_all();

    if (cyw43_arch_init() != PICO_OK) {
        printf("CYW43 initialization failed.\n");
        return 1;
    }

    const BaseType_t result = xTaskCreate(
        main_task,
        "Main",
        MAIN_TASK_STACK_SIZE,
        NULL,
        MAIN_TASK_PRIORITY,
        NULL
    );

    if (result != pdPASS) {
        printf("Unable to create Main task.\n");
        cyw43_arch_deinit();
        return 1;
    }

    vTaskStartScheduler();

    /* A correctly configured scheduler never returns. */
    printf("FreeRTOS scheduler stopped unexpectedly.\n");
    cyw43_arch_deinit();
    return 1;
}
