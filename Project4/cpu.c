#include <stdio.h>
#include "task.h"
void run(struct task* task, int time)
{
    printf("[Task = %s] [Priority = %d] [Executing time = %d]\n", task->name, task->priority, time);
}