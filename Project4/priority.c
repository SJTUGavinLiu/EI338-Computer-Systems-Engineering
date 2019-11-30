#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scheduler.h"
#include "vector.h"
#include "task.h"
#include "cpu.h"






void add(char *name, int priority, int burst)
{
    struct task* newTask = malloc(sizeof(struct task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    

    struct node *p = head->next;
    
    if(!p || p->task->priority < newTask->priority)
    {
        addTohead(newTask);
    }
    else
    {
        struct node* tmp;
        while(p)
        {
            //printf("make");
            tmp = p->next;
            if(!tmp)    addTotail(newTask);
            else if(tmp->task->priority < newTask->priority)
            {
                insert(p,newTask);
                break;
            }
            p = tmp;
        }

    }
    
    
}


void schedule()
{
    while(!isEmpty())
    {
        run(head->next->task, head->next->task->burst);
        deleteHead();
    }
}