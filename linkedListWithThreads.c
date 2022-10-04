//1 thread = search
//1 thread = remove end node
//1thread = add end node

#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int count = 0;
//declare lock variable;
pthread_mutex_t lock; //pthread_mutex_t is the lock variable type //can name it to anything


class Node
{
        public:
                int data;
                Node *next;

                Node(int udata)//constructor
                {
                        data = udata;
                        next = NULL;
                }
};

class Linkedlist
{
        Node *head; //private member can be accessed by a public member in the same class

        public:
                Linkedlist(int udata) //constructor
                {
                        head = new Node(udata);//creates head node
                }

                void add(int udata)
                {
                        Node *newnode = new Node(udata); //creates new node and creates a pointer pointing to that node
                        Node *temp = head; //creates temp node pointer that points to head
                        while (temp->next != NULL)      //moves temp pointer until it reaches the end of linked list//arrow operator since temp is a pointer
                        {
                                temp = temp->next;
                        }
                        temp->next = newnode; //sets the pointer of the node that temp is pointing to to point at the new node

                }

                void search(int udata)//finds a node that has udata
                {
                        Node *temp = head;
                        while (temp->data != udata && temp->next != NULL)
                        {
                                temp = temp->next;

                        }
                        if(temp->data == udata)
                        {
                                cout << temp->data << endl;
                        }
                        else
                        {
                                cout << "data doesnt exist" << endl;
                        }
                }

                void remove()//removes last node
                {
                        Node *temp = head;
                        Node *temp2 = head;
                        while (temp->next != NULL)
                        {
                                temp = temp->next;
                        }
                        while (temp2->next != temp)//while the pointer of the node that temp2 is pointing at is not == to the address of the node that temp is pointing at
                        {
                                temp2 = temp2->next;
                        }
                        temp2->next = NULL;
                        delete temp;
                }

                void print()
                {
                        Node *temp = head; //makes temp point to head
                        while (temp->next != NULL) // loop to go through each node starting from head
                        {
                                cout << temp->data << "->";//prints the current node's data
                                temp = temp->next;
                        }
                        cout << temp->data << endl;// prints the last node's data
                }

                ~Linkedlist()
                {
                        Node *temp = head;
                        Node *temp2 = temp;

                        while(temp->next != NULL)
                        {
                                temp2 = temp;
                                temp = temp->next;
                                delete temp2;
                        }
                        delete temp;
                }
};

Linkedlist *mylist = new Linkedlist(0); // create linkedlist

void *threadAdd(void *arg) //thread function
{
        pthread_mutex_lock(&lock); //locks section from other threads
        int id = *(int *) arg;
        int data;

        printf("TID: %d\n", id); //print thread id
        cout <<"what is the node data?\n";
        cin >> data;

        mylist->add(data);
        pthread_mutex_unlock(&lock); // unlocks section above for other threads

        pthread_exit(NULL); //thread exits
}

void *threadRemove(void *arg) //thread function
{
        pthread_mutex_lock(&lock); //locks section from other threads
        int id = *(int *) arg;
        int data;

        printf("TID: %d\n", id); //print thread id
        mylist->remove();
        pthread_mutex_unlock(&lock); // unlocks section above for other threads

        pthread_exit(NULL); //thread exits
}

void *threadSearch(void *arg) //thread function
{
        pthread_mutex_lock(&lock); //locks section from other threads
        int id = *(int *) arg;
        int data;

        printf("TID: %d\n", id); //print thread id
        cout <<"what is the node data?\n";
        cin >> data;

        mylist->search(data);
        pthread_mutex_unlock(&lock); // unlocks section above for other threads

        pthread_exit(NULL); //thread exits
}

void *threadPrint(void *arg) //thread function
{
        pthread_mutex_lock(&lock); //locks section from other threads
        int id = *(int *) arg;

        printf("TID: %d\n", id); //print thread id

        mylist->print();
        pthread_mutex_unlock(&lock); // unlocks section above for other threads

        pthread_exit(NULL); //thread exits
}

int main()
{
        const int numthrds = 4; //number of threads
        int idx[numthrds] = {0,1,2,3}; //thread index to tell which thread i running
        pthread_t tid[numthrds]; //stores id of thread
        pthread_mutex_init (&lock, NULL); //initialize lock
        int ans = -1;

        /*
        for (int i=0; i<numthrds; i++)
        {
                pthread_create (&tid[i], NULL, threadfun, (void *)&idx[i]);
        }
        */

        while (ans != 0) //input loop
        {
                cout << "1 = add node, 2 = remove last node, 3 = search node, 4 = print linked list , 0 = exit program " << endl;
                cin >> ans;

                while (ans > 4 || ans < 0)
                {
                        cout << "invalid answer. Try again\n";
                        cin >> ans;
                }


                if (ans == 1)//add
                {
                        pthread_create (&tid[0], NULL, threadAdd, (void *)&idx[0]);
                        pthread_join(tid[0], NULL);
                }
                else if (ans == 2)//remove
                {
                        pthread_create (&tid[1], NULL, threadRemove, (void *)&idx[1]);
                        pthread_join(tid[1], NULL);
                }
                else if (ans == 3)//search
                {
                        pthread_create (&tid[2], NULL, threadSearch, (void *)&idx[2]);
                        pthread_join(tid[2], NULL);
                }
                else if (ans == 4)//print
                {
                        pthread_create (&tid[3], NULL, threadPrint, (void *)&idx[3]);
                        pthread_join(tid[3], NULL);
                }
                else if (ans == 0)//exit
                {
                        cout << "exiting program...\n";
                }

        }
        /*
        for (int i = 0; i<numthrds; i++)
        {
                pthread_join(tid[i], NULL); //holds all threads after they finish
        }
        */
        pthread_mutex_destroy(&lock); // destroy lock to free up space taken by lock
        //printf("MAIN: COUNT %d\n", count );


        delete mylist;

        return 0;
}

