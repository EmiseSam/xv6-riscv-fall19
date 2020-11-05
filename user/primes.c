#include "kernel/types.h"
#include "user/user.h"


void prime(int *n, int length)
{
    if (length == 1)
    {
        printf("prime %d\n", *n);
        return;
    }

    int temp = *n;
    printf("prime %d\n", temp);

    int i, j;
    int p1[2];
    pipe(p1);

    for (i = 0; i < length; i++)
    {
        j = *(n + i);
        if (write(p1[1], (int *)&j, 4) != 4)
        {
            printf("f--->s write error!\n");
            exit();
        }
    }

    int childpid = fork();
    close(p1[1]);
    if (childpid == 0)
    {
        int count = 0;
        int buffer[4];
        while (read(p1[0], buffer, 4) != 0)
        {
            j = *((int *)buffer);
            if (j % temp != 0)
            {
                *n = j;
                n++;
                count++;
            }
        }
        prime(n - count, count);
        exit();
    }
    wait();
    wait();
    wait();
}
int main()
{
    int number[34];
    int i = 0;
    for (; i < 34; i++)
    {
        number[i] = i + 2;
    }
    prime(number, 34);
    exit();
}