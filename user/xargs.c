#include "kernel/param.h" 
#include "kernel/types.h"
#include "user/user.h"

int main(int argc,char *argv[])
{
    char buf[1000];
    char *argv_exec[MAXARG];
    int i;
    if((argc < 2) || (argc + 1 > MAXARG))
    {
        printf("arguments fail\n");
        exit(-1);
    }
    for (int j = 1; j < argc ;j++)
        argv_exec[j-1] = argv[j];
    while(1)
    {
        i = 0;
        while(1)
        {
            if(read(0, buf + i,sizeof(char)) < 0)
                break;
            if(buf[i] == '\n')
                break;
            i++;
        }
        if(i == 0)
            break;
        buf[i] = 0;
        argv_exec[argc - 1] = buf;
        argv_exec[argc] = 0;
        if(fork() == 0)
        {
            exec(argv_exec[0],argv_exec);
            exit(0);
        }
        else{
            wait(0);
        }
    }
    exit(0);
}
