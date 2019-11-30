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
        struct node *end;
        end = head->next;
        int priority = end->task->priority;
        // find tasks that have same priority
        while(end->next)
        {
            if(end->next->task->priority == priority)
                end =end->next;
            else
                break;
        }
        
        struct node* tmp1 = tail;
        struct node* tmp2 = end->next;
        tail = end;
        tail->next = NULL;
        
        int time = 0;
        while(!isEmpty())
        {
            time = min(10,head->next->task->burst);
            run(head->next->task, time);
            head->next->task->burst -= time;
            if(head->next->task->burst == 0)
                deleteHead();
            else
                moveTotail();
        }

        if(!tmp2)   break;
        else
        {
            head->next = tmp2;
            tail = tmp1;  
        }
        
        
    }
}



/*
void schedule()
{
    while(!isEmpty())
    {
        struct node *end;
        end = head->next;
        int priority = end->task->priority;
        // find tasks that have same priority
        while(end->next)
        {
            if(end->next->task->priority == priority)
                end =end->next;
            else
                break;
        }
        


        struct node* p = head;
        int time;

        while(1)
        {
            time = min(10,p->next->task->burst);
            run(p->next->task, time);
            p->next->task->burst -= time;
            if(p->next->task->burst == 0)
            {
                if(p->next == end) end = p;
                delete(p);
            }  
            if(end == head) break;  
            p = p->next == end ? head : p->next;
        }
    }
}
*/
