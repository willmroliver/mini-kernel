#include "ring.h"
#include <core/mem.h>

struct ring *ring_alloc(u32 size)
{
	// size must be a power of 2
	if (size & (size - 1))
		return 0;
		
	struct ring *ring = kmalloc(sizeof(struct ring) + size);
	ring->buf = (u8 *)(ring) + sizeof(struct ring);
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

int ring_push(struct ring *ring, u8 byte)
{
	if ((ring->end - ring->start) == ring->size)
		return 0;
	
	ring->buf[ring->end & ring->mask] = byte;
	ring->end++;
	return 1;
}

int ring_pop(struct ring *ring, u8 *byte)
{
	if (ring->end == ring->start)
		return 0;

	*byte = ring->buf[ring->start & ring->mask];
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

u8 ring_front(struct ring *ring)
{
	return ring->buf[ring->start & ring->mask];
}

u8 ring_back(struct ring *ring)
{
	return ring->buf[(ring->end - 1) & ring->mask];
}
