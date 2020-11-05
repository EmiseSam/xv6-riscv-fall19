#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int i, j, pid, len;
    int first_blank;

    // MAXARG=32  在param.h声明
    char args[MAXARG][32];
    // 三十二个指针
    char *p[MAXARG];
    char buf;

    // 参数数量不足
    if (argc < 2)
    {
        fprintf(2, "usage: xargs <cmd> ...\n");
        exit();
    }

    while (1)
    {
        // init清零
        i = 0;
        // 记录空格所在是否为首位（首位忽视）
        first_blank = 0;
        // 初始化数组清零
        memset(args, 0, MAXARG * 32);

        // 将命令参数和stdin参数推送到一起
        for (j = 1; j < argc; j++)
        {
            // 除了命令本身，其余均copy到agrs中
            strcpy(args[i++], argv[j]);
        }
        
        j = 0;
        while (i < MAXARG - 1)
        {
            if ((len = read(0, &buf, 1)) <= 0)
            {
                // CTRL+D
                wait();
                exit();
            }
            // 一行读取结束
            if (buf == '\n')
            {
                break;
            }
            // 空格判读
            if (buf == ' ')
            {
                if (first_blank)
                {
                    // 非首位空格换行
                    i++;
                    j = 0;
                    first_blank = 0;
                }
                continue;
            }
            // 读入
            args[i][j++] = buf;
            first_blank = 1;
        }

        for (i = 0; i < MAXARG - 1; i++)
        {
            p[i] = args[i];
        }
        // 最后一个参数必须是0
        p[MAXARG - 1] = 0;

        // 执行exec命令
        if ((pid = fork()) == 0)
        {
            exec(argv[1], p);
            exit();
        }
    }
    exit();
}