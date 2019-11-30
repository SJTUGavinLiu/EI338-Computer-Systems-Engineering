#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include "buffer.h"


#define NUM_OF_PRODUCER 3
#define NUM_OF_CONSUMER 3
#define MAX_SLEEP_PERIOD 3


pthread_mutex_t mutex;
sem_t empty;
sem_t full;




void *producer(void *param){
    buffer_item item;

    while(1){
        // sleep for a random period of time
        sleep(rand() % MAX_SLEEP_PERIOD + 1);

        item = rand();

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);


        if(insert_item(item))
            fprintf(stderr, "error in producer");
        else
            printf("producer produced %d\n", item);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *param){
    buffer_item item;

    while(1){
        // sleep for a random period of time
        sleep(rand() % MAX_SLEEP_PERIOD + 1);

        item = rand();

        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        if(remove_item(&item))
            fprintf(stderr, "error in consumer");
        else
            printf("consumer consumed %d\n", item);
        
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        
    }
}






int main()
{
    // initialization of mutex and semaphore
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);


    pthread_t producers[NUM_OF_PRODUCER];
    pthread_t consumers[NUM_OF_CONSUMER];

    for(int i = 0; i < NUM_OF_PRODUCER; i++){
        pthread_create(&producers[i], NULL, producer, NULL);
    }
    for(int i = 0; i < NUM_OF_CONSUMER; i++){
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }


    sleep(10);


     for(int i = 0; i < NUM_OF_PRODUCER; i++){
        pthread_cancel(producers[i]);
        pthread_join(producers[i], NULL);
    }
    for(int i = 0; i < NUM_OF_CONSUMER; i++){
        pthread_cancel(consumers[i]);
        pthread_join(consumers[i], NULL);
    }


    //free(producers);
    //free(consumers);
    return 0;


}