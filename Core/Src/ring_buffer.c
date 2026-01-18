#include "ring_buffer.h"



bool ring_buffer_push(uint8_t byte, RingBuffer *rb) {
	uint32_t next_head =(rb->head +1) % rb->size;
	if(rb->tail == next_head) {
		return false;
	}
	rb->data[rb->head] = byte;
	rb->head = next_head;
	return true;
}

bool ring_buffer_pop(uint8_t *byte, RingBuffer *rb) {
	if (rb->head == rb->tail) {
		return false;
	}
	*byte = rb->data[rb->tail];
	rb->tail = (rb->tail +1) % rb->size;
	return true;
}
