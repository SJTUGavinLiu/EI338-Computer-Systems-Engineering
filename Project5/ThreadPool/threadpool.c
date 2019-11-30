#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include "threadpool.h"


#define QUEUE_SIZE 10
#define THREADS 5
#define bool int

#define true 1
#define false 0





struct task{
    void (*function)(void *p);
    void *data;
};





struct task queue[QUEUE_SIZE];

int head = 0;
int tail = 0;

pthread_t pool[THREADS];


pthread_mutex_t mutex;      // for the queue
sem_t sem;





bool isFull()
{ 
    return head == (tail + 1) % QUEUE_SIZE;
}

bool isEmpty()
{
    return head == tail;
}




int enqueue(struct task t)
{
    pthread_mutex_lock(&mutex);
    if(isFull())
    {
        // The queue is full
        pthread_mutex_unlock(&mutex);
        return 1;
    }
    queue[tail] = t;
    tail = (tail + 1) % QUEUE_SIZE;
    pthread_mutex_unlock(&mutex);
    return 0;
}


struct task* dequeue()
{
    pthread_mutex_lock(&mutex);
    if(isEmpty())
    {
        // The queue is empty
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    struct task *t = &queue[head];
    head = (head + 1) % QUEUE_SIZE;
    pthread_mutex_unlock(&mutex);
    return t;
}


void *worker(void *param)
{
    while(1)
    {
        sem_wait(&sem);
        struct task* next = dequeue();
        if(next == NULL)    continue;
        execute(next->function, next->data);
    }
    pthread_exit(0);
}


void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}


void pool_init(void)
{
    sem_init(&sem, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i < THREADS; i++)
    {
        pthread_create(&pool[i],NULL,worker,NULL);
    }
}

int pool_submit(void (*somefunction)(void *p), void *p)
{
    struct task t;
    t.function = somefunction;
    t.data = p;

    sem_post(&sem);
    return enqueue(t);
}

void pool_shutdown(void)
{
    for(int i = 0; i < THREADS; i++)
    {
    	pthread_cancel(pool[i]);
        pthread_join(pool[i],NULL);
    }
}

