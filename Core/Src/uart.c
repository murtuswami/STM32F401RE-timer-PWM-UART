#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#include "ring_buffer.h"



static uint8_t rx_storage[RING_BUFFER_SIZE];
static RingBuffer rx_rb = {
    .data = rx_storage,
    .size = RING_BUFFER_SIZE,
    .head = 0,
    .tail = 0,
};

static uint8_t tx_storage[RING_BUFFER_SIZE];
static RingBuffer tx_rb = {
    .data = tx_storage,
    .size = RING_BUFFER_SIZE,
    .head = 0,
    .tail = 0,
};


bool poll_uart_rx(uint8_t data_buf[],uint32_t buf_size){
	uint8_t b;
	uint32_t data_index = 0;
	while(ring_buffer_pop(&b,&rx_rb)) {
		if (data_index >= buf_size -1) { // stop 2 index before to leave a null term character
			data_buf[buf_size -1 ] = '\0';
			return false; // overflow
		}
		data_buf[data_index++] = b;
	}
	data_buf[data_index] = '\0';
	return true;
}

bool push_uart_tx(const uint8_t *data_buf) {
	while((char)*data_buf) {
		if (!ring_buffer_push(*data_buf, &tx_rb)) return false;
		++data_buf;
	}

	USART2->CR1 |= USART_CR1_TXEIE;
	return true;
}


void UART_IRQ_custom_handler(){
	if (USART2->SR & USART_SR_RXNE) { 	// recieve buffer
		uint8_t b = USART2->DR;
		ring_buffer_push(b,&rx_rb);
	}
	if (USART2->SR & USART_SR_TXE) {	// send buffer
		uint8_t b;
		if(ring_buffer_pop(&b, &tx_rb)){
			USART2->DR = b;
		}
		else{
			USART2->CR1 &= ~USART_CR1_TXEIE; // tx rb empty, disable send interrupts
		}
	}
}


