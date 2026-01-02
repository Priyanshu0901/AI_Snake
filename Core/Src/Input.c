/*
 * Input.c
 *
 *  Created on: 02-Jan-2026
 *      Author: rayv_mini_pc
 */

#include "Input.h"

void INPUT_ctor(INPUT_t *const me, KEYPAD_t *keypad) {
	me->keypad = keypad;
	me->last_raw_key = NO_KEY;
	me->key_processed = false;
}

key_action_e INPUT_get_action(INPUT_t *const me) {
    keys_e current_raw = me->keypad->key;

    // 1. Handle Release: Reset state when no key is pressed
    if (current_raw == NO_KEY) {
        me->key_processed = false;
        me->last_raw_key = NO_KEY;
        return ACTION_NONE;
    }

    // 2. Prevent "Machine Gun" Auto-Repeat:
    // If we already processed this specific key, don't trigger it again
    if (me->key_processed && current_raw == me->last_raw_key) {
        return ACTION_NONE;
    }

    // 3. Process New Action
    key_action_e action = ACTION_NONE;

    // Use the flag OR the state change to trigger the action
    if (me->keypad->new_key_press || current_raw != me->last_raw_key) {
        me->keypad->new_key_press = false; // Reset hardware flag
        me->key_processed = true;          // Mark as processed for this hold
        me->last_raw_key = current_raw;    // Save for next comparison

        switch (current_raw) {
            case S2:  action = ACTION_UP;    break;
            case S10: action = ACTION_DOWN;  break;
            case S5:  action = ACTION_LEFT;  break;
            case S7:  action = ACTION_RIGHT; break;
            case S6:  action = ACTION_CONFIRM; break;
            default:  action = ACTION_NONE;  break;
        }
    }

    return action; // Always returns a valid enum
}
