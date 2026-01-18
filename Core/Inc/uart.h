#pragma once
#include <stdint.h>
#include <stdbool.h>
bool poll_uart_rx(uint8_t *data_buf,uint32_t buf_size);

bool push_uart_tx(uint8_t *data_buf);


void UART_IRQ_custom_handler();
