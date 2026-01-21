#pragma once

#include <stdint.h>
#include <stdbool.h>

#define RING_BUFFER_SIZE 256


typedef struct {
    uint8_t *data;
    uint32_t size;
    uint32_t head;
    uint32_t tail;
} RingBuffer;


bool ring_buffer_push(uint8_t byte, RingBuffer *rb);
bool ring_buffer_pop(uint8_t *byte, RingBuffer *rb);
