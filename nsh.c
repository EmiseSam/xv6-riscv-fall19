// Shell.
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

//参数最大数量
#define MAXARGS 10
//每个参数最大长度
#define MAXWORD 10

//无效字符：水平制表符、回车符、换行符、垂直制表符
char whitespace[] = " \t\r\n\v";
//存储一条命令中的每个参数
char args[MAXARGS][MAXWORD];

//获取用户输入的命令
int getcmd(char *buf, int nbuf)
{
    fprintf(2, "@ ");
    memset(buf, 0, nbuf);
    //从缓冲区获取一串字符
    gets(buf, nbuf);
    if (buf[0] == 0) // EOF
        return -1;
    return 0;
}

//分离命令中的每个参数，同时省略无效字符
void setargs(char *cmd, char *argv[], int *argc)
{
    // 将argv设立成args中每个参数的指针
    for (int i = 0; i < MAXARGS; i++)
    {
        argv[i] = &args[i][0];
    }
    // 表示第i个word
    int i = 0;
    int j = 0;
    //找到命令中的每一个参数
    for (; cmd[j] != '\n' && cmd[j] != '\0'; j++)
    {
        //去掉无效字符
        while (strchr(whitespace, cmd[j]))
        {
            j++;
        }
        //每个命令参数作为字符串的第一个字符的地址赋值给对应的argv
        argv[i++] = cmd + j;

        // 只要不是空格，就j++,找到下一个空格，两个空格间就是一个参数
        while (strchr(whitespace, cmd[j]) == 0)
        {
            j++;
        }
        cmd[j] = '\0';
    }
    //最后一个参数argv[size-1]为0
    argv[i] = 0;
    //存储参数的总数量
    *argc = i;
}

int main(void)
{
    static char buf[100];
    int fd;

    while (getcmd(buf, sizeof(buf)) >= 0)
    {

        if (fork() == 0)
        {
            char *argv[MAXARGS];
            int argc = -1;
            setargs(buf, argv, &argc);
            runcmd(argv, argc);
        }
        wait(0);
    }
    exit(0);
}