#include "main.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

static volatile uint32_t period = 0;
static volatile uint32_t high = 0;
static volatile bool sample_ready = false;

void tim2_pwm_start_ch1(void) {

    // Enable CH1 output
    TIM2->CCER |= TIM_CCER_CC1E;

    // Optional: force update so ARR/CCR preload values latch
    TIM2->EGR = TIM_EGR_UG;

    // Start timer
    TIM2->CR1 |= TIM_CR1_CEN;

}

void tim3_input_pwm_start_ch1_ch2 () {
	TIM3->DIER |= TIM_DIER_CC1IE;


    TIM3->CCER |= TIM_CCER_CC1E;
    TIM3->CCER |= TIM_CCER_CC2E;


    TIM3->CR1 |= TIM_CR1_CEN;

}

void set_duty_cycle(uint32_t new_pulse) {

	TIM2->CCR1 = new_pulse;

}

bool consume_sample(uint32_t * period_ptr, uint32_t * high_ptr) {
	if(sample_ready) {
		TIM3->DIER &= ~TIM_DIER_CC1IE;   // stop CC1 interrupt
		*period_ptr = period;
		*high_ptr = high;
		sample_ready = false;
		TIM3->DIER |= TIM_DIER_CC1IE;    // re-enable
		return true;
	}
	return false;
}

void TIM3_IRQ_custom_handler() {
	if(TIM3->SR & TIM_SR_CC1IF){

        TIM3->SR &= ~TIM_SR_CC1IF;   // ✅ clear flag

		period = TIM3->CCR1;  // full period
		high = TIM3->CCR2; // high time
		sample_ready = true;

	}


}

/*
volatile uint32_t cc1_count = 0;
volatile uint32_t cc2_count = 0;


void TIM3_IRQ_custom_handler(void)
{
    if (TIM3->SR & TIM_SR_CC1IF) { // rising
        TIM3->SR &= ~TIM_SR_CC1IF;
        cc1_count++;
    }
    if (TIM3->SR & TIM_SR_CC2IF) { // falling
        TIM3->SR &= ~TIM_SR_CC2IF;
        cc2_count++;
    }
}*/

