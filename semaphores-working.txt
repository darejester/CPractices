#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define size 5 //number of slots in buffer
#define num_threads 2 //number of threads
#define num_items 10 //number of items

int buf[size] = {};
int idx; //points at the index of buffer (how much of it is filled)
int value; //
sem_t sem_full;
sem_t sem_empty;

void add(int element)//adds int to buffer
{
        if (idx < size) //can only add if idx is less than size (only when not full)
        {
                buf[idx] = element;
                idx++;
        }
        else
        {
                printf("buffer is full = overflow\n");
        }
}

void delete(int element)//consumes elements that are inside the buffer
{
        if (buf[idx] >= 0) //there is still an element in index 0
        {
                idx--;
        }
        else
        {
                printf("buffer is empty = underflow\n");
        }
}

void *produce(void *arg)//producer (adds)
{
        for (int i = 0; i<num_items; i++)
        {
                sem_wait(&sem_empty); // if sem_empty == 0, it locks the code
                
                value = rand()%100; // 0 to 100
                add(value);
                
                sem_post(&sem_full); // increments
                
                printf("producer: added %d\n",value);
        }
        pthread_exit(NULL);
}

void *consume(void *arg)//consumer (deletes)
{
        for (int i = 0; i<num_items; i++)
        {
                sem_wait(&sem_full);
                
                delete(value);
                
                sem_post(&sem_empty);
                
                printf("consumer: removed %d\n", value);
                
        }
        pthread_exit(NULL);
}

int main()
{
        pthread_t pid;//producer thread
        pthread_t cid;//consumer thread
        idx = 0;
        sem_init(&sem_empty, 1, 1);//sem_empty = 1 if buffer is empty, 0 if not empty
        sem_init(&sem_full, 1, 0);//sem_full = 1 if buffer is full, 0 if not full 
        
        pthread_create(&pid, NULL, produce, NULL);
        pthread_create(&cid, NULL, consume, NULL);
        
        pthread_join(cid,NULL);
        pthread_join(pid,NULL);
        
        return 0;
        
}
