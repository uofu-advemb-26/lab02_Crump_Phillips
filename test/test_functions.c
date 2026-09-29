#include <stdio.h>

#include "functions.h"
#include "pico/stdlib.h"
#include "unity.h"
#include "unity_config.h"

void setUp(void)
{
}

void tearDown(void)
{
}

static void test_lowercase_a_becomes_uppercase_a(void)
{
    TEST_ASSERT_EQUAL_INT('A', toggle_ascii_case('a'));
}

static void test_lowercase_z_becomes_uppercase_z(void)
{
    TEST_ASSERT_EQUAL_INT('Z', toggle_ascii_case('z'));
}

static void test_uppercase_a_becomes_lowercase_a(void)
{
    TEST_ASSERT_EQUAL_INT('a', toggle_ascii_case('A'));
}

static void test_uppercase_z_becomes_lowercase_z(void)
{
    TEST_ASSERT_EQUAL_INT('z', toggle_ascii_case('Z'));
}

static void test_digit_is_unchanged(void)
{
    TEST_ASSERT_EQUAL_INT('5', toggle_ascii_case('5'));
}

static void test_punctuation_is_unchanged(void)
{
    TEST_ASSERT_EQUAL_INT('?', toggle_ascii_case('?'));
}

static void test_blink_state_initializes_to_known_values(void)
{
    blink_state_t state = { .iteration = 99U, .led_on = true };

    blink_state_init(&state);

    TEST_ASSERT_EQUAL_UINT32(0U, state.iteration);
    TEST_ASSERT_FALSE(state.led_on);
}

static void test_iteration_zero_outputs_off_and_holds_next_level(void)
{
    blink_state_t state = { .iteration = 0U, .led_on = false };

    const bool output = blink_step(&state);

    TEST_ASSERT_FALSE(output);
    TEST_ASSERT_EQUAL_UINT32(1U, state.iteration);
    TEST_ASSERT_FALSE(state.led_on);
}

static void test_regular_iteration_outputs_current_level_then_toggles(void)
{
    blink_state_t state = { .iteration = 1U, .led_on = false };

    const bool output = blink_step(&state);

    TEST_ASSERT_FALSE(output);
    TEST_ASSERT_EQUAL_UINT32(2U, state.iteration);
    TEST_ASSERT_TRUE(state.led_on);
}

static void test_multiple_of_eleven_outputs_current_level_and_holds(void)
{
    blink_state_t state = { .iteration = 11U, .led_on = true };

    const bool output = blink_step(&state);

    TEST_ASSERT_TRUE(output);
    TEST_ASSERT_EQUAL_UINT32(12U, state.iteration);
    TEST_ASSERT_TRUE(state.led_on);
}

int main(void)
{
    stdio_init_all();

    for (;;) {
        sleep_ms(5000U);
        printf("\nstarting tests\n");

        UNITY_BEGIN();
        RUN_TEST(test_lowercase_a_becomes_uppercase_a);
        RUN_TEST(test_lowercase_z_becomes_uppercase_z);
        RUN_TEST(test_uppercase_a_becomes_lowercase_a);
        RUN_TEST(test_uppercase_z_becomes_lowercase_z);
        RUN_TEST(test_digit_is_unchanged);
        RUN_TEST(test_punctuation_is_unchanged);
        RUN_TEST(test_blink_state_initializes_to_known_values);
        RUN_TEST(test_iteration_zero_outputs_off_and_holds_next_level);
        RUN_TEST(test_regular_iteration_outputs_current_level_then_toggles);
        RUN_TEST(test_multiple_of_eleven_outputs_current_level_and_holds);
        (void)UNITY_END();
    }
}
