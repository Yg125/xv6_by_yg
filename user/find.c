#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    buf[strlen(p)] = 0;
    //memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

void find(char *path, char *filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_FILE:
        if(strcmp(fmtname(path),filename) == 0)
            printf("%s\n",path);
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/'; //p就是path/，之后该目录下的文件可以表示为在p之后加文件名
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            else if (strcmp(de.name,".") == 0 || strcmp(de.name,"..") == 0)
                continue;
            memmove(p, de.name, DIRSIZ);  //抛开. 和 .. 得到的目录中的文件路径
            p[DIRSIZ] = 0;
            
            find(buf,filename); //buf是完整路径
        } 
        break;
    }
    close(fd);
    return;
}
 
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("find needs at least three arguments\n");
        exit(-1);
    }
    find(argv[1], argv[2]);
    exit(0);
}