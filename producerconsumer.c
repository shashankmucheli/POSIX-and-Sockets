/*
Name: Shashank Mucheli Sukumar
ID: 01442857
Course: CIS 570
Instructor: Dr. Paul Gracia

Instructions to compile the code: 
	1. Open terminal in an unix environment.
	2. type: gcc producerconsumer.c
	3. type: ./a.out <sleeptime> <number of producer threads> <number of consumer threads>
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define SIZE 10

pthread_mutex_t mutex;
sem_t full, empty;
int count = 0;

typedef int buffer_item;

buffer_item buffer[SIZE];

pthread_t pt, ct;

void *producer(void *arg) {
	buffer_item item;
	int i = 0;
	while(1) {
		sleep((rand() % 10));
		item = (rand() % RAND_MAX);

		sem_wait(&empty);
			pthread_mutex_lock(&mutex);
				if(insert_item(item) == 0) {
					printf("[Pthread:%d] Producing %d \n", arg, item);
				} 
				else{
					printf("Buffer Full\n");
				}
			pthread_mutex_unlock(&mutex);
		sem_post(&full);

		if (i % 2 == 1) {
			sleep(1);
			i++;
		}
	}
}

void *consumer(void *arg) {
	buffer_item item;
	int i = 0;
	while(1) {
		sleep(rand() % 10);
	
		sem_wait(&full);
			pthread_mutex_lock(&mutex);
				if(remove_item(&item) == 0) {
					printf("[Cthread:%d] Consuming %d \n", arg, item);
				}
				else{
					printf("Buffer Empty\n");
				}
			pthread_mutex_unlock(&mutex);
		sem_post(&empty);
		
		if (i % 2 == 0) {
			sleep(1);
			i++;
		}
	}
}

void initialize(){
	pthread_mutex_init(&mutex, NULL);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, SIZE);	
}

int insert_item(buffer_item item) {
   if(count < SIZE) {
      buffer[count] = item;
      count++;
      return 0;
   }
   else {
      return -1;
   }
}

int remove_item(buffer_item *item) {
   if(count > 0) {
      *item = buffer[(count-1)];
      count--;
      return 0;
   }
   else { 
      return -1;
   }
}

int main(int argc, char *argv[]) {
	int i;
	srand( time( NULL ) );
	
	if(argc < 3){
		printf("Invalid number of arguments\nMinimum 3 arguments: \n1. Sleeptime\n2. Number of producer threads\n3. Number of consumer threads\n");
		exit(0);
	}
	
	int sleeptime = atoi(argv[1]);
	int p = atoi(argv[2]);
	int c = atoi(argv[3]);
	
	initialize();
   
	for(i = 0; i < p; i++) {
		pthread_create(&pt,NULL,producer,(void *)(intptr_t)i);
	}

	for(i = 0; i < c; i++) {
		pthread_create(&ct,NULL,consumer,(void *)(intptr_t)i);
	}

	sleep(sleeptime);
	printf("Program executed for %d seconds\n", sleeptime);
	exit(0);
	
	pthread_exit(NULL);
}