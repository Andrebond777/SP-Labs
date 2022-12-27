#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

void printProc()
{
    printf("pid - %d\n", getpid());
    printf("pid - %d\n", getsid(getpid()));
    printf("pid - %d\n\n", getgid());
}

int main(int argc, char **argv, char **envp)
{
    pid_t pid;

    printf("Begin parent proc\n");
    printProc();

    if ((pid = fork()) < 0)
        exit(1);

    if (pid != 0)
    {
        for (int i = 0; i < 10; i++)
        {
            printf("Parent proc\n");
            printProc();
            sleep(1);
        }

        if (waitpid(pid, NULL, 0) != pid)
            exit(1);
    }
    else
    {
        for (int i = 0; i < 10; i++)
        {
            printf("Child  proc\n");
            printProc();
            sleep(1);
        }
    }

    exit(0);
}
