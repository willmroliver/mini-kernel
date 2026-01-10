#include "ring.h"
#include <core/mem.h>



struct ring *ring_alloc(u32 size)
{
	// size must be a power of 2
	if (~(size - 1))
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

int ring_push(struct ring *ring, u8 byte)
{
	if (ring->end - ring->start == ring->size)
		return 0;
	
	ring->buf[ring->end & ring->mask] = byte;
	ring->end++;
	return 1;
}

int ring_pop(struct ring *ring, u8 *byte)
{
	if (ring->end == ring->start)
		return 0;

	ring->buf[ring->start & ring->mask] = *byte;
	ring->start++;
	return 1;
}
