#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
int main(int argc, char *argv[])
{
    char buf[1000];
    char *argv_exec[MAXARG];
    int i;
    if((argc < 2) || (argc + 1 > MAXARG))
    {
        printf("arguments error!\n");
        exit(1);
    }
    for(i=1 ; i < argc ; i++){
        argv_exec[i-1] = argv[i];//argv[0]是xargs,而传入exec的参数数组只需要之后的参数
    }
    while (1) {
        i = 0;
        while (1) {
            if(read(0, &buf[i], sizeof(char)) < 0) //从标准输入0中读入字符，如果出错，那么就跳出循环
                break;
            if(buf[i] == '\n') //直到读到换行符再执行之后的程序
                break;
            i++;
        }
        if(i == 0) break;
        buf[i] = 0;//将buf最后一个值赋为0，表示字符串
        argv_exec[argc-1] = buf;/*将buf放到argc_exec的末尾*/
        argv_exec[argc] = 0;/*按要求argv_exec最后一个元素为0*/
        if(fork() == 0){
            exec(argv_exec[0], argv_exec);//argv_exec[0]就是要执行的命令，并将刚才得到的buf连同最开始的参数一起作为参数传入exec执行
            exit(0);
        } else {
            wait(0);
        }
    }
    exit(0);
}

