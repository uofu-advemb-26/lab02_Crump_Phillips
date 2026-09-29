#include <stdio.h>

#include "functions.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "unity.h"
#include "unity_config.h"

void setUp(void)
{
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
}

void tearDown(void)
{
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
}

static void test_low_blink_output_is_applied_to_led_gpio(void)
{
    blink_state_t state = { .iteration = 1U, .led_on = false };
    const bool expected_level = blink_step(&state);

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, expected_level);

    TEST_ASSERT_FALSE(cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN));
}

static void test_high_blink_output_is_applied_to_led_gpio(void)
{
    blink_state_t state = { .iteration = 1U, .led_on = true };
    const bool expected_level = blink_step(&state);

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, expected_level);

    TEST_ASSERT_TRUE(cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN));
}

int main(void)
{
    stdio_init_all();
    sleep_ms(3000U);

    const int init_result = cyw43_arch_init();
    if (init_result != PICO_OK) {
        printf("CYW43 initialization failed: %d\n", init_result);
        return init_result;
    }

    for (;;) {
        sleep_ms(5000U);
        printf("\nStarting Lab 2 hardware integration tests\n");

        UNITY_BEGIN();
        RUN_TEST(test_low_blink_output_is_applied_to_led_gpio);
        RUN_TEST(test_high_blink_output_is_applied_to_led_gpio);
        (void)UNITY_END();
    }
}
