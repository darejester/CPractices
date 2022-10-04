#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define buf_size 20
#define num_customers 200

int buffer1[buf_size] = {};
int buffer2[buf_size] = {};
int buffer3[buf_size] = {};
int idx1 = 0;
int idx2 = 0;
int idx3 = 0;
int value;
int pctr = 0; //times produced
int cctr = 0; //times consumed
sem_t sem_full1;
sem_t sem_empty1;
sem_t sem_full2;
sem_t sem_empty2;
sem_t sem_full3;
sem_t sem_empty3;

//declare lock variable;
pthread_mutex_t lock; //pthread_mutex_t is the lock variable type //can name it to anything

void add1(int element)
{
	//this function will add an element to the buffer and perform index correction
	if (idx1 < buf_size) //can only add if idx is less than size (only when not full)
    {
        buffer1[idx1] = element;
        idx1++;
    }
	else
	{
		printf("buffer is full = overflow\n");
	}
}

void delete1(int element)
{
	//this funtion will delete an element to the buffer and perform index correction
	if (buffer1[idx1] >= 0) //there is still an element in index 0
    {
        idx1--;
    }
    else
    {
        printf("buffer is empty = underflow\n");
    }
}

void add2(int element)
{
	//this function will add an element to the buffer and perform index correction
	if (idx2 < buf_size) //can only add if idx is less than size (only when not full)
    {
        buffer2[idx2] = element;
        idx2++;
    }
	else
	{
		printf("buffer is full = overflow\n");
	}
}

void delete2(int element)
{
	//this funtion will delete an element to the buffer and perform index correction
	if (buffer2[idx2] >= 0) //there is still an element in index 0
    {
        idx2--;
    }
    else
    {
        printf("buffer is empty = underflow\n");
    }
}

void add3(int element)
{
	//this function will add an element to the buffer and perform index correction
	if (idx3 < buf_size) //can only add if idx is less than size (only when not full)
    {
        buffer3[idx3] = element;
        idx3++;
    }
	else
	{
		printf("buffer is full = overflow\n");
	}
}

void delete3(int element)
{
	//this funtion will delete an element to the buffer and perform index correction
	if (buffer3[idx3] >= 0) //there is still an element in index 0
    {
        idx3--;
    }
    else
    {
        printf("buffer is empty = underflow\n");
    }
}

/*
void *produce(void *arg)
{
	//call add here within mutex
	pthread_mutex_lock(&lock); //locks section from other threads
	for (int i = 0; i<num_elements; i++)
    {
        sem_wait(&sem_empty); // if sem_empty == 0, it locks the code
        
        value = rand()%100; // 0 to 100
        add(value);
        pctr++;
        
        sem_post(&sem_full); // increments
        
        printf("producer: added %d\n",value);
    }
    pthread_mutex_unlock(&lock); // unlocks section above for other threads
    pthread_exit(NULL);
}

void *consume(void *arg)
{
	//call delete here within mutex
	//pthread_mutex_lock(&lock); //locks section from other threads
	for (int i = 0; i<100; i++)
    {
        sem_wait(&sem_full);
        
        delete(value);
        cctr++;
        
        sem_post(&sem_empty);
        
        printf("consumer: removed %d\n", value);     
    }
    //pthread_mutex_unlock(&lock); // unlocks section above for other threads
    pthread_exit(NULL);
}
*/

void *takeOrder(void *arg)
{
	//pthread_mutex_lock(&lock); //locks section from other threads
	for (int i = 0; i<num_customers; i++)
    {
        sem_wait(&sem_empty1); // decrements //if sem_empty == 0, it locks the code
        
        value = rand()%100; // 0 to 100
        add1(value);
        pctr++;
        
        printf("PRODUCE: %d\n",value);
        
        sem_post(&sem_full1); // increments
        
    }
    //pthread_mutex_unlock(&lock); // unlocks section above for other threads
    pthread_exit(NULL);
}

void *cookOrder(void *arg)
{
	
	for (int i = 0; i<num_customers; i++)
    {
    	//pthread_mutex_lock(&lock); //locks section from other threads
        sem_wait(&sem_full1); // decrements 
        
        value = rand()%100; // 0 to 100
        delete1(value);
        cctr++;
        
        printf("CONSUME: %d\n",value);
        
        sem_post(&sem_empty1); // increments
        //pthread_mutex_unlock(&lock); // unlocks section above for other threads
        
        //////////////////////////////////////////////////////////////////////////////
        
        //pthread_mutex_lock(&lock); //locks section from other threads
        sem_wait(&sem_empty2); // decrements 
        
        value = rand()%100; // 0 to 100
        add2(value);
        pctr++;
        
        printf("PRODUCE: %d\n",value);
        
        sem_post(&sem_full2); // increments
        //pthread_mutex_unlock(&lock); // unlocks section above for other threads
        
    }
    pthread_exit(NULL);
}
void *packOrder(void *arg)
{
	for (int i = 0; i<num_customers; i++)
    {
    	//pthread_mutex_lock(&lock); //locks section from other threads
        sem_wait(&sem_full2); // decrements 
        
        value = rand()%100; // 0 to 100
        delete2(value);
        cctr++;
        
        printf("CONSUME: %d\n",value);
        
        sem_post(&sem_empty2); // increments
        //pthread_mutex_unlock(&lock); // unlocks section above for other threads
        
        //////////////////////////////////////////////////////////////////////////////
        
        //pthread_mutex_lock(&lock); //locks section from other threads
        sem_wait(&sem_empty3); // decrements 
        
        
        value = rand()%100; // 0 to 100
        add3(value);
        pctr++;
        
        printf("PRODUCE: %d\n",value);
        
        sem_post(&sem_full3); // increments
        //pthread_mutex_unlock(&lock); // unlocks section above for other threads
        
    }
    //pthread_mutex_unlock(&lock); // unlocks section above for other threads
    pthread_exit(NULL);
}
void *sellOrder(void *arg)
{
	//pthread_mutex_lock(&lock); //locks section from other threads
	for (int i = 0; i<num_customers; i++)
    {
        sem_wait(&sem_full3); // decrements 
        
        value = rand()%100; // 0 to 100
        delete3(value);
        cctr++;
        
        printf("CONSUME: %d\n",value);
        
        sem_post(&sem_empty3); // increments
    }
    //pthread_mutex_unlock(&lock); // unlocks section above for other threads
    pthread_exit(NULL);
}

int main()
{
	pthread_mutex_init (&lock, NULL); //initialize lock
	
	pthread_t taker; //order takers
	pthread_t cook; //takes orders from order takers, cooks food
	pthread_t pack; //takes cooked food, packs food
	pthread_t cash; //takes packed food, sells sells
	
	//initiate all semaphores here
	sem_init(&sem_empty1, 1, 1); //sem_empty = 1 if buffer is empty, 0 if not empty //initialized as empty
	sem_init(&sem_full1, 1, 0); //sem_full = 1 if buffer is full, 0 if not full  //initialized as not full
	sem_init(&sem_empty2, 1, 1); //sem_empty = 1 if buffer is empty, 0 if not empty //initialized as empty
	sem_init(&sem_full2, 1, 0); //sem_full = 1 if buffer is full, 0 if not full  //initialized as not full
	sem_init(&sem_empty3, 1, 1); //sem_empty = 1 if buffer is empty, 0 if not empty //initialized as empty
	sem_init(&sem_full3, 1, 0); //sem_full = 1 if buffer is full, 0 if not full  //initialized as not full
	
	//create threads
	pthread_create(&taker, NULL, takeOrder,NULL);
	pthread_create(&cook, NULL, cookOrder, NULL);
	pthread_create(&pack, NULL, packOrder, NULL);
	pthread_create(&cash, NULL, sellOrder, NULL);

	pthread_join(taker, NULL); 
	pthread_join(cook, NULL); 
	pthread_join(pack, NULL); 
	pthread_join(cash, NULL);
	
	pthread_mutex_destroy(&lock); // destroy lock to free up space taken by lock
	
	printf("produce count: %i \n", pctr);
	printf("consume count: %i \n", cctr);
}


