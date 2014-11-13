#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include </usr/include/semaphore.h>
#include <time.h>

// for sleep
#include <unistd.h>

#define BUFF_SIZE   10
typedef struct
{
    int buffer_item[BUFF_SIZE];  
    int in;              
    int out;             
    sem_t full;          
    sem_t empty;         

    // use correct type here
    pthread_mutex_t mutex;
} buf;

buf shared;

void *insert_item(void *arg)
{
    int i, item, index;

    //index = (int)arg;
	int value, max;
	max = (signed int) RAND_MAX;
    sem_getvalue(&shared.empty, &value); 
    printf("The value of the producer semaphors is %d\n %d", value, max);


    //for (i=0; i < value; i++)
    for (i=0; i < value; i++)
    {
        /* Produce item */
        item = (rand()%max);

        /* Prepare to write item to buf */

        /* If there are no empty slots, wait */
        sem_wait(&shared.empty);
        /* If another thread uses the buffer, wait */
        pthread_mutex_lock(&shared.mutex);
        shared.buffer_item[shared.in] = item;
        shared.in = (shared.in+1)%BUFF_SIZE;
        printf("[P%d] Producing %d \n", arg, item);
        fflush(stdout);
		sem_getvalue(&shared.empty, &value);
        /* Release the buffer */
        pthread_mutex_unlock(&shared.mutex);
        /* Increment the number of full slots */
        sem_post(&shared.full);

        /* Interleave  producer and consumer execution */
        if (i % 2 == 1) sleep(1);
    }
    return NULL;
}

void *remove_item(void *arg)
{
    int i, item, index;

    //index = (int)arg;
	
	int value; 
    sem_getvalue(&shared.full, &value); 
    printf("The value of the consumer semaphors is %d\n", value);
	
    //for (i=value; i > 0; i--) { 
    for (i=value; i > 0; i--) { 
		sem_wait(&shared.full);
        pthread_mutex_lock(&shared.mutex);
        item=i;
        item=shared.buffer_item[shared.out];
        shared.out = (shared.out+1)%BUFF_SIZE;
        printf("[C%d] Consuming  %d \n", arg, item);
        fflush(stdout);
		sem_getvalue(&shared.full, &value);
        /* Release the buffer */
        pthread_mutex_unlock(&shared.mutex);
        /* Increment the number of full slots */
        sem_post(&shared.empty);

        /* Interleave  producer and consumer execution */
        if (i % 2 == 1) sleep(1);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
	srand(time(0));
	printf("number 1: %s\n", argv[1]);
	printf("number 2: %s\n", argv[2]);
	printf("number 3: %s\n", argv[3]);
		
	pthread_t pt, ct;
    int index, p, c, sleeptime;
	int status;
	sleeptime = atoi(argv[1]);
	p = atoi(argv[2]);
	c = atoi(argv[3]);

	printf("The process id is %d \n", getpid());
	
    sem_init(&shared.full, 0, 1);
    sem_init(&shared.empty, 0, BUFF_SIZE);
    pthread_mutex_init(&shared.mutex, NULL);
    for (index = 1; index <= p; index++)
    {
        /* Create a new producer */
        pthread_create(&pt, NULL, insert_item, (void *)index);
    }
    /*create a new Consumer*/
    for(index=1; index <= c; index++)
    {
        pthread_create(&ct, NULL, remove_item, (void *)index);
    }
	
	
	sleep(sleeptime);
	printf("Program executed for %d seconds\n", sleeptime);
	exit(0);

    pthread_exit(NULL);


}