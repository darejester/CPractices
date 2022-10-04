#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int done = 0; //1 = done, 0 = not done, still working
pthread_mutex_t lock;
pthread_cond_t c;//conditional variable

void mythrd_exit()
{
	pthread_mutex_lock(&lock);
	done = 1;
	pthread_cond_signal(&c); //send signal to the parent thread that the child is done
	pthread_mutex_unlock(&lock);
}

void mythrd_join()
{
	pthread_mutex_lock (&lock);
	while(done == 0)//while child is running
	{
		pthread_cond_wait(&c, &lock);//release the lock and waits for the signal, for the condition to be satisfied
	}
	pthread_mutex_unlock(&lock);
}

void *child(void *arg)
{
	printf("CHILD THREAD\n"); //child thread prints "child thread"
	mythrd_exit();
	return NULL;
}



int main()
{
	pthread_t c;
	printf("PARENT THREAD: Start\n");
	pthread_create(&c, NULL, child, NULL);
	mythrd_join();//parent thread waits on child thread
	printf("PARENT THREAD: End\n");
	
	return 0;
}
