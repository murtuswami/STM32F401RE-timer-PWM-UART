#pragma once
#include <stdint.h>
#include <stdbool.h>
void tim2_pwm_start_ch1(void);
void set_duty_cycle(uint32_t);
void tim3_input_pwm_start_ch1_ch2();
bool consume_sample(uint32_t * period_ptr, uint32_t * high_ptr) ;
void TIM3_IRQ_custom_handler(void);
