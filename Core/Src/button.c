#include "button.h"
#include "main.h"
#include <stdbool.h>

#define NUMBER_OF_DEBOUNCE_SAMPLES 5
#define HELD_TICKS 1000 //400 ticks == 400ms

static bool debounce_pressed = false;


typedef enum {
    RELEASED,
    PRESSED,
    HELD
} ButtonState;



static uint32_t held_consecutive_count = 0;
static ButtonState button_state = RELEASED;
static bool long_press_event = false;
static bool short_press_event = false;



static void button_debounce();
static void update_button_state(uint32_t);
static bool consume_short_press_event();
static bool consume_long_press_event();

/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_13) {
	  b1_event = 1;

  }
}*/

ButtonEvent update_button(uint32_t g_ticks){
	 button_debounce();
	 update_button_state(g_ticks);
	 if(consume_short_press_event()) {
		 return SHORT_PRESS;
	  }

	 if(consume_long_press_event()) {
		 return LONG_HOLD_THRESHOLD_CROSSED;
	  }
	 return NO_EVENT;
}

static void update_button_state(uint32_t g_ticks) {
    static uint32_t l_ticks;
    if (debounce_pressed) { 			//pressed
        switch (button_state){
            case RELEASED:              //RELEASE =>PRESSED
                button_state = PRESSED;
                l_ticks = g_ticks;
                held_consecutive_count = 1;
                break;
            case PRESSED:
                if(held_consecutive_count < HELD_TICKS) {    //RELEASE =>
                    uint32_t passed_ticks = g_ticks - l_ticks;
                    button_state = PRESSED;
                    l_ticks = g_ticks;
                    held_consecutive_count += passed_ticks;
                } else {                          //PRESSED ==> HELD
                    long_press_event = true;
                    button_state = HELD;
                }
                break;
            case HELD:      //HELD ==> HELD
                break;
        }

    }
    else { //released
        switch (button_state){
        case RELEASED:                  //RELEASED  ==> RELEASED
            break;
        case PRESSED:
            button_state = RELEASED;    // PRESSED ==> RELEASED
            held_consecutive_count = 0;
            short_press_event = true;
            break;
        case HELD:                      // HELD   ==> RELEASED
            button_state = RELEASED;
            held_consecutive_count= 0;
            break;
        }
    }

}



static void button_debounce() {
	static uint32_t last_button_raw_state = 0; //assume not pressed on program start.
	static uint32_t button_raw_stable_count = 0;

	uint32_t debounce_sample = (GPIOC->IDR & GPIO_IDR_ID13) ? 1u : 0u; // 0 if value is 0, 1 if value is 1
    //handle debounce check and trim stable count to max samples
    if (debounce_sample == last_button_raw_state ) {
        	button_raw_stable_count += 1; // keep stable count ceiling to number of debounce samples
    } else {
        button_raw_stable_count = 1;
        last_button_raw_state = debounce_sample;
    }
    // check if we have are 'debounce pressed'
    if (button_raw_stable_count >= NUMBER_OF_DEBOUNCE_SAMPLES) {
    	button_raw_stable_count = NUMBER_OF_DEBOUNCE_SAMPLES; // clamp stable count
        debounce_pressed = (last_button_raw_state == 0); // low voltage when pressed
    }

}


static bool consume_short_press_event() {
    if (short_press_event) {
        short_press_event = false;
        return true;
    } else {
        return false;
    }
}

static bool consume_long_press_event() {
      if (long_press_event) {
        long_press_event = false;
        return true;
    } else {
        return false;
    }
}
