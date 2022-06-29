#include "kernel/types.h"
#include "user/user.h"

#define R 0
#define W 1

void Pipe(int fd[2])  //include the false detection
{
    if (pipe(fd) == -1)
    {
        printf("pipe fail\n");
        exit(-1);
    }
}
int Fork()          //include the false detection
{
    int pid = fork();
    if (pid < 0)
    {
        printf("fork fail\n");
        exit(-1);
    }
    return pid;
}

void Child_proc(int fd[2])
{
    close(fd[W]);
    int fd_child[2];
    pipe(fd_child);
    int prime;
    int testnum;
    if (read(fd[R], &prime, sizeof(int)) == 0)    //if there is number in the pipe
        return;
    printf("prime %d\n", prime);

    int pid = Fork();
    if (pid == 0)
    {
        Child_proc(fd_child);
        exit(0);
    }
    else
    {
        while (read(fd[R], &testnum, sizeof(int)) != 0)
        {
            if (testnum % prime != 0)
                write(fd_child[W], &testnum, sizeof(int)); 
        }
        close(fd[R]);
        close(fd_child[W]);
        wait(0);
        return;
    }
}

int main()
{
    int fd[2];
    Pipe(fd);
    int pid = Fork();
    if (pid == 0)
    {
        Child_proc(fd);
        exit(0);
    }
    else
    {

        close(fd[R]);
        for (int i = 2; i < 36; i++)
            write(fd[W], &i, sizeof(int)); //write every elements into pipe
        close(fd[W]); // after finishing write ,close it
        wait(0);      // wait all the child process exit
        exit(0);
    }
    return 0;
}