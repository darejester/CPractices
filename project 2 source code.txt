#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
        int pc[2] = {0,1};//0 is write, 1 is read
        int cp[2] = {0,1};//0 is write, 1 is read
        int ppid = getpid();
        int y = 6;
        int x = 5; 
        pipe(pc);
        pipe(cp);

        printf("x: %d, y = %d\n", x,y);

        if (fork() == 0)//child
        {
                y++;
                printf("Child: updated y to: %d\n", y);
                close(cp[0]);
                write(cp[1], &y,sizeof(y));
                close(pc[1]);
                read(pc[0], &x, sizeof(x));
                printf("Child: recieving updated x from parent...\n");
                printf("Child: parent's updated x is: %d\n", x);
        }
        else//parent
        {
                close(cp[1]);
                read(cp[0], &y, sizeof(y));
                printf("Parent: recieving updated y from child...\n");
                printf("Parent: child's updated y is: %d\n", y);
                x++;
                printf("Parent: updated x to: %d\n", x);
                close(pc[0]);
                write(pc[1], &x, sizeof(x));

        }
}
