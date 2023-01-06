#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#define CHMOD (0644)
#define BUFFERLENGTH (1024)

int logFile;

void signal_handler(int signo, siginfo_t* si, void* ucontext)
{
    size_t  memorySize = snprintf(NULL, 0, "si_signo = %d\nsi_pid = %d\nsi_uid = %d\nsi_errno = %d\n",
        si->si_signo, si->si_pid, si->si_uid, si->si_errno) + sizeof('\0');
    char* str = malloc(memorySize);

    snprintf(str, memorySize, "si_signo = %d\nsi_pid = %d\nsi_uid = %d\nsi_errno = %d\n",
        si->si_signo, si->si_pid, si->si_uid, si->si_errno);

    if (signal(signo, SIG_DFL) == SIG_ERR)
        exit(1);
    else if (raise(signo) == -1)
        exit(1);
    else if ((write(logFile, str, strlen(str))) == -1)
        exit(1);
}

int main(int argc, char** argv)
{
    if ((logFile = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, CHMOD)) == -1)
        exit(1);

    size_t  memorySize = snprintf(NULL, 0, "Program started started\n") + sizeof('\0');

    char* str = malloc(memorySize);
    snprintf(str, memorySize, "Program started started\n");

    if ((write(logFile, str, strlen(str))) == -1)
        exit(1);

    struct sigaction sig;
    sig.sa_flags = SA_RESTART ^ SA_SIGINFO;
    sig.sa_sigaction = &signal_handler;
    sigaction(SIGHUP, &sig, NULL);

    while (1)
    {
        memorySize = snprintf(NULL, 0, "pid = %d sid = %d gid = %d\n", getpid(), getsid(getpid()), getgid()) + sizeof('\0');

        str = malloc(memorySize);
        snprintf(str, memorySize, "pid = %d sid = %d gid = %d\n", getpid(), getsid(getpid()), getgid());

        if ((write(logFile, str, strlen(str))) == -1)
            exit(1);

        sleep(10);
    }

    exit(0);
}