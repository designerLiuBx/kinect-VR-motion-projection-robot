#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "sys.h"
#include "Gen_include.h"

typedef struct
{
void(*task_func)(void);
u32 uLenTick;
u32 uNextTick;
}sched_task_t;

void Scheduler_Setup(void);
void Scheduler_Run(void);

#endif
