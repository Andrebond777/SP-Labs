#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char **argv, char **envp)
{
    pid_t pid;

    printf("Parent process started:\npid = %d;\ngid = %d;\nsid = %d.\n\n", getpid(), getgid(), getsid(getpid()));

    if ((pid = fork()) < 0)
    {
        printf("Error with fork()\n%s\n", strerror(errno));
        exit(1);
    }
    else if (pid == 0) // Child
    {
        for (int i = 0; i < 4; i++)
        {
            printf("Child process:\npid = %d;\ngid = %d;\nsid = %d.\n\n", getpid(), getgid(), getsid(getpid()));
            sleep(2);
        }
        printf("Child process finished! (pid = %d)\n", getpid());
    }
    else               // Parent
    {
        for (int i = 0; i < 3; i++)
        {
            printf("Parent process:\npid = %d;\ngid = %d;\nsid = %d.\n\n", getpid(), getgid(), getsid(getpid()));
            sleep(2);
        }

        if (waitpid(pid, NULL, 0) != pid)
        {
            printf("Error with waitpid()\n%s\n", strerror(errno));
            exit(1);
        }
        printf("Parent process finished! (pid = %d)\n", getpid());
    }

    exit(0);
}
