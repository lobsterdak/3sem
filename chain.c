#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    int n, i, id, pid;
    printf("Введите длину цепочки\n");
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            printf ("%d %d \n", getppid(), getpid());
            continue;
        }
        wait(&id);
        exit(1);
    }
    return 0;
}
