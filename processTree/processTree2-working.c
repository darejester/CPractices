#include <stdio.h>
#include <unistd.h>
#include <signal.h>

//A--B
//A--C
//B--D
//C--E


void child(int idx, pid_t parent, int level){
        int i; pid_t cpid[3]; pid_t l_parent = getpid();
        if(idx == 0 && level == 1 ){//this is process B
                for(i=0; i<1; i++)//here B creates D
				{
                        if( (cpid[i]=fork()) == 0){
                                child(i,l_parent,2);
                        }
                }
                //B will run this
                sleep(5);
                for(i= 0; i<1; i++)
				{
                        kill(cpid[i], SIGKILL);
                }
                printf("MY_PID:%d, PARENT_PID: %d\n", getpid(), parent);
                sleep(5);
        }
        else if(idx == 1 && level == 1 ){//this is process C
                for(i=0; i<1; i++)//here C creates E
				{
                    if( (cpid[i]=fork()) == 0){
                            child(i,l_parent,2);
                    }
                }
                //C will run this
                sleep(5);
                for(i= 0; i<1; i++){
                        kill(cpid[i], SIGKILL);
                }
                printf("MY_PID:%d, PARENT_PID: %d\n", getpid(), parent);
                sleep(5);
        }
        else{//D,E
                printf("MY_PID:%d, PARENT_PID=%d\n", getpid(), parent);
                sleep(10);
        }
}


int main(){//process A
        pid_t cpid[3];
        pid_t parent = getpid();
        int i;

        for(i=0; i<2; i++){
                if( (cpid[i]=fork()) == 0){//B,C
                        child(i,parent,1);//this function creates child processes
                }
        }

        //process A will run this code
        sleep(10);//A sleeps for 10 seconds

        for(i=0; i<2; i++){
                kill(cpid[i], SIGKILL); //A kills B, C
        }

        sleep(5);//this is for synchronization (extra)
        printf("A:PID:%d\n", getpid());
        return 0;
}
