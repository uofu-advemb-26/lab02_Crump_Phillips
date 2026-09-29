/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 * Edited by Matthew Crump and Will Phillips
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "functions.h"

#include <assert.h>
#include <stddef.h>

#define BLINK_HOLD_PERIOD 11U

void blink_state_init(blink_state_t *state)
{
    assert(state != NULL);

    state->iteration = 0U;
    state->led_on = false;
}

bool blink_step(blink_state_t *state)
{
    assert(state != NULL);

    const bool output_level = state->led_on;

    if ((state->iteration % BLINK_HOLD_PERIOD) != 0U) {
        state->led_on = !state->led_on;
    }

    state->iteration++; // this will wrap around to zero with it reaches UINT32 MAX

    return output_level;
}

int toggle_ascii_case(int character)
{
    if ((character >= 'a') && (character <= 'z')) {
        return character - ('a' - 'A');
    }

    if ((character >= 'A') && (character <= 'Z')) { // much much more readable than the original version, we are just swapping case.
        return character + ('a' - 'A');
    }

    return character;
}
