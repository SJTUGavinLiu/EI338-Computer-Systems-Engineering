#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scheduler.h"
#include "vector.h"
#include "task.h"
#include "cpu.h"


#define min(a,b)    (((a) < (b)) ? (a) : (b))

void add(char *name, int priority, int burst)
{
    struct task* newTask = malloc(sizeof(struct task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    //printf(name);
    addTotail(newTask);
    //addTohead(newTask);
}



void schedule()
{
    int time = 0;
    while(!isEmpty())
    {
        time = min(5,head->next->task->burst);
        run(head->next->task, time);
        head->next->task->burst -= time;
        if(head->next->task->burst == 0)
            deleteHead();
        else
            moveTotail();
    }
}