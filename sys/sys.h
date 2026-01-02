#ifndef INC_SYS_H
#define INC_SYS_H

#include <core/types.h>

void __init_exception_handlers(void **sys_handlers);

struct sys_frame_ix {
	vaddr_t (*at)(struct sys_frame_ix *);			
	vaddr_t (*ret)(struct sys_frame_ix *);
	vaddr_t (*stack)(struct sys_frame_ix *);
	int (*exception)(struct sys_frame_ix *);
};

static inline vaddr_t sys_frame_at(struct sys_frame_ix *frame)
{
	return frame->at(frame);
}

static inline vaddr_t sys_frame_ret(struct sys_frame_ix *frame)
{
	return frame->ret(frame);
}

static inline vaddr_t sys_frame_stack(struct sys_frame_ix *frame)
{
	return frame->stack(frame);
}

static inline int sys_frame_exception(struct sys_frame_ix *frame)
{
	return frame->exception(frame);
}

#endif
