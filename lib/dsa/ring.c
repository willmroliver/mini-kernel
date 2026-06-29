#include "ring.h"
#include <core/mem.h>

struct ring8 *ring8_alloc(u32 size)
{
	// size must be a power of 2
	if (size & (size - 1))
		return 0;
		
	struct ring8 *ring = kmalloc(sizeof(struct ring8) + size);
	ring->buf = (u8 *)(ring) + sizeof(struct ring8);
	ring->size = size;
	ring->mask = size - 1;
	ring->start = 0;
	ring->end = 0;

	return ring;
}

void ring8_free(struct ring8 *ring)
{
	kfree(ring);
}

int ring8_push(struct ring8 *ring, u8 byte)
{
	if ((ring->end - ring->start) == ring->size)
		return 0;
	
	ring->buf[ring->end & ring->mask] = byte;
	ring->end++;
	return 1;
}

int ring8_pop(struct ring8 *ring, u8 *byte)
{
	if (ring->end == ring->start)
		return 0;

	*byte = ring->buf[ring->start & ring->mask];
	ring->start++;
	return 1;
}

int ring8_full(struct ring8 *ring)
{
	return (ring->end - ring->start == ring->size);
}

int ring8_empty(struct ring8 *ring)
{
	return ring->start == ring->end;
}

u32 ring8_size(struct ring8 *ring)
{
	return ring->end - ring->start;
}

u8 ring8_front(struct ring8 *ring)
{
	return ring->buf[ring->start & ring->mask];
}

u8 ring8_back(struct ring8 *ring)
{
	return ring->buf[(ring->end - 1) & ring->mask];
}

struct ring *ring_alloc(u32 size)
{
	// size must be a power of 2
	if (size & (size - 1))
		return 0;
		
	struct ring *ring = kmalloc(sizeof(struct ring) + size);
	ring->buf = (void *)(ring) + sizeof(struct ring);
	ring->size = size;
	ring->mask = size - 1;
	ring->start = 0;
	ring->end = 0;

	return ring;
}

void ring_free(struct ring *ring)
{
	kfree(ring);
}

int ring_push(struct ring *ring, void *ptr)
{
	if ((ring->end - ring->start) == ring->size)
		return 0;
	
	ring->buf[ring->end & ring->mask] = ptr;
	ring->end++;
	return 1;
}

int ring_pop(struct ring *ring, void *ptr)
{
	if (ring->end == ring->start)
		return 0;

	ptr = ring->buf[ring->start & ring->mask];
	ring->start++;
	return 1;
}

int ring_full(struct ring *ring)
{
	return (ring->end - ring->start == ring->size);
}

int ring_empty(struct ring *ring)
{
	return ring->start == ring->end;
}

u32 ring_size(struct ring *ring)
{
	return ring->end - ring->start;
}

void *ring_front(struct ring *ring)
{
	return ring->buf[ring->start & ring->mask];
}

void *ring_back(struct ring *ring)
{
	return ring->buf[(ring->end - 1) & ring->mask];
}
