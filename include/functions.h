/**
 * @file app_logic.h
 * @brief Testable behavior used by the Lab 2 FreeRTOS application.
 *
 * The RTOS tasks and Pico hardware calls do not happen here.
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** State retained between blink loop iteration. */
typedef struct {
    uint32_t iteration;
    bool led_on;
} blink_state_t;

/** Initialize blink state to a known poweron state. */
void blink_state_init(blink_state_t *state);

/**
 * Execute one iteration of the blink behavior.
 *
 * The return value is the level that the caller should write to the LED iteration.  The state is then advanced for the next iteration
 * and it changes once out of 11 iterations.
 *
 * @param state Nonnull state owned by the caller.
 * @return The LED level to write.
 */
bool blink_step(blink_state_t *state);

/**
 * Swap the case of an ASCII letter and leave every other integer unchanged.
 * we use an int so nothing gets truncated and can return EOF if needed.
 */
int toggle_ascii_case(int character);

#ifdef __cplusplus
}
#endif

#endif
