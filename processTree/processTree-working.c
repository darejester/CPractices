//A--B
//A--C
//B--E
//B--D
//C--F

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void child(int idx, pid_t parent, int level){
        int i; pid_t cpid[3]; pid_t l_parent = getpid();
        if(idx == 0 && level == 1 )
        {//this is process B
                for(i=0; i<2; i++)
                {//here B creates E,D
                        if( (cpid[i]=fork()) == 0)
                        {//E,D //B calls fork //E D executes if's instructions
                                child(i,l_parent,2);
                        }
                }

                 //B will run this
                sleep(5);
                for(i= 0; i<2; i++){
                        kill(cpid[i], SIGKILL);
                }
                printf("MY_PID:%d, PARENT_PID: %d\n", getpid(), parent);
                sleep(5);
        }
        else if(idx ==1 && level ==1)//process C
        {
                for(i=0; i<1; i++)
                {//here B creates E,D
                        if ((cpid[i]=fork()) == 0)
                        {
                                child(0,l_parent,2); //c creates F
                        }
                }

                //c will run this
                sleep(5);
                for(i= 0; i<1; i++){
                        kill(cpid[i], SIGKILL);
                }
                printf("MY_PID:%d, PARENT_PID: %d\n", getpid(), parent);

        }
        else{//E,D,F
                printf("MY_PID:%d, PARENT_PID=%d\n", getpid(), parent);
                sleep(10);
        }
}


int main(){//process A
        pid_t cpid[3]; //stores pid of children in an array
        pid_t parent = getpid();
        int i;

        for(i=0; i<2; i++){
                if( (cpid[i]=fork()) == 0){//B, C and D //i = 0. A will call fork, which creates B and C. A's child will execute if's instructions
                        child(i,parent,1);//this function creates child processes
                }
        }
        //process A will run this code
                sleep(10);//A sleeps for 10 seconds
        for(i=0; i<2; i++){
                kill(cpid[i], SIGKILL); //A kills B, C and D
        }

        sleep(5);//this is for synchronization (extra)
        printf("A:PID:%d\n", getpid());//prints pid of A
        return 0;
}
