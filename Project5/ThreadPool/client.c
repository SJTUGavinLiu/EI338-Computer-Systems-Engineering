/*
    A sample of the program that uses thread pool.
*/

#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"

struct data
{
    int a;
    int b;
};

void add(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    printf("I multiply two values %d and %d result = %d\n",temp->a, temp->b, temp->a * temp->b);
}

int main(void)
{
    // create some work to do
    struct data work;
    work.a = 5;
    work.b = 10;

    // initialize the thread pool
    pool_init();

    // submit the work to the queue
    for (int i=0; i<20; i++)
    {
        int s = pool_submit(&add,&work);
        while (s)
        {
           s = pool_submit(&add,&work);
        }
        //__sync_fetch_and_add(&work.a,1);
        //__sync_fetch_and_add(&work.b,1);
        //work.a++;
        //work.b++;
    }

    // may be helpful 
    sleep(3);

    pool_shutdown();

    return 0;
}
