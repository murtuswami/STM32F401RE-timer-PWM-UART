#pragma once
#include <stdint.h>
#include <stdbool.h>


typedef enum {
    SHORT_PRESS,
	LONG_HOLD_THRESHOLD_CROSSED,
	NO_EVENT,
} ButtonEvent;


ButtonEvent update_button(uint32_t g_ticks);




