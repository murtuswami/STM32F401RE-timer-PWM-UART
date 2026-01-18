#pragma once
#include <stdint.h>
#include "button.h"

typedef enum {
    LED_BLINK,
    LED_ON,
	LED_OFF,
} LedState;




void update_led(ButtonEvent,uint32_t);

