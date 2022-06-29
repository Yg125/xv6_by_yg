#include "kernel/types.h"
#include "user/user.h"

#define R 0
#define W 1
char buffer[10];
int main()
{
    int pid;
    int fd1[2],fd2[2];
    if(pipe(fd1) == -1 || pipe(fd2) == -1){
        printf("Pipe failed\n");
        exit(-1);
    }
    pid = fork();
    if(pid < 0)
    {
        printf("fork fail\n");
        exit(-1);
    }
    else if(pid == 0)  //child process
    {
        close(fd1[W]);
        close(fd2[R]);
        read(fd1[R],buffer,sizeof(buffer));
        int pid = getpid();
        printf("%d: received %s\n",pid,buffer);
        close(fd1[R]);
        write(fd2[W],"pong",sizeof("pong"));
        close(fd2[W]);
        exit(0);
    }
    else{   //parent process
        close(fd1[R]);
        close(fd2[W]);
        write(fd1[W],"ping",sizeof("ping"));
        close(fd1[W]);
        wait(0);
        read(fd2[R],buffer,sizeof(buffer));
        int pid = getpid();
        printf("%d: received %s\n",pid,buffer);
        close(fd2[R]);
        exit(0);
    }
    return 0;
}