#include "main.h"
#include "led.h"
#include "button.h"
#include <stdbool.h>

#define BLINK_PERIOD 500 //1000MS = 1s

static LedState led_on();
static LedState led_off();
static void toggle_led();



static LedState led_state = LED_OFF;
static uint32_t l_ticks= 0;
static bool blink_flag = false;



void update_led(ButtonEvent btn_event, uint32_t now){
    switch (btn_event){
        case SHORT_PRESS:
				blink_flag = false;
				if(led_state == LED_ON) {
					led_state = led_off() ;
				}
				else if(led_state == LED_OFF) {
					led_state = led_on();
				}
				else {	// blink
					led_state = led_off();
				}
				break;
        case LONG_HOLD_THRESHOLD_CROSSED:
        		if (led_state == LED_BLINK) {
        			led_state = led_off();
        		}
        		else {
        			led_state = LED_BLINK;
        		}
            break;
        case NO_EVENT:
            break;
    }
    if(led_state == LED_BLINK && (now - l_ticks >= BLINK_PERIOD)){
    	toggle_led();
    	l_ticks = now;
    }

 }

static LedState led_on() {
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_SET);
	return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5) == GPIO_PIN_SET ? LED_ON : LED_OFF;
}

static LedState led_off() {
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_RESET);
	return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5) == GPIO_PIN_RESET ? LED_OFF : LED_ON;
}


static void toggle_led() {

	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_SET);
	}

}



