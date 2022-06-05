#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
  if (argc != 2) { //参数错误
    printf("Sleep needs one argument!\n");
    exit(1);
  }
    int ticks=atoi(argv[1]);//将字符串参数转为整数
    sleep(ticks);
    printf("nothing happens for a little while\n");
    exit(0);//确保进程退出
}
