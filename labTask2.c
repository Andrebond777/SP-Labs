#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

#define SIZEBUF (1024)
#define CHMOD (0644)

int main(int argc, char** argv, char** envp)
{
	pid_t pid;
	int logFile;

	if ((logFile = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, CHMOD)) == -1)
	{
		printf("Error with open(log.txt, ...)\n%s\n", strerror(errno));
		exit(1);
	}

	if ((write(logFile, "Parent process started\n", sizeof("Parent process started\n") - 1)) == -1)
	{
		printf("Error with write()\n%s\n", strerror(errno));
		exit(1);
	}

	if ((pid = fork()) < 0)
	{
		printf("Error with fork()\n%s\n", strerror(errno));
		exit(1);
	}
	else if (pid == 0) // Child
	{
		int devNull;
		char* str;
		size_t memNeeded;
		char* path = malloc(SIZEBUF * sizeof * path);
		getcwd(path, SIZEBUF);

		if (setsid() == -1)
		{
			printf("Error with setsid()\n%s\n", strerror(errno));
			exit(1);
		}

		if (chdir("/") == -1)
		{
			printf("Error with chdir()\n%s\n", strerror(errno));
			exit(1);
		}

		if (close(logFile) == -1)
		{
			printf("Error with close()\n%s\n", strerror(errno));
			exit(1);
		}

		if ((devNull = open("/dev/null", O_WRONLY)) == -1)
		{
			printf("Error with open(/dev/null, ...)\n%s\n", strerror(errno));
			exit(1);
		}

		if (dup2(devNull, 0) == -1) // stdin
		{
			printf("Error with dup2(devNull, 0)\n%s\n", strerror(errno));
			exit(1);
		}

		if (dup2(devNull, 1) == -1) // stdout
		{
			printf("Error with dup2(devNull, 1)\n%s\n", strerror(errno));
			exit(1);
		}

		if (dup2(devNull, 2) == -1) // stderr
		{
			printf("Error with dup2(devNull, 2)\n%s\n", strerror(errno));
			exit(1);
		}

		strcat(path, "/log.txt");

		if ((logFile = open(path, O_WRONLY | O_APPEND, CHMOD)) == -1)
		{
			printf("Error with open()\n%s\n", strerror(errno));
			exit(1);
		}

		if ((write(logFile, "Child process was created\n", sizeof("Child process was created\n") - 1)) == -1)
		{
			printf("Error with write()\n%s\n", strerror(errno));
			exit(1);
		}

		while (1)
		{
			memNeeded = snprintf(NULL, 0, "Child process:\npid = %d;\ngid = %d;\nsid = %d.\n\n", getpid(), getgid(), getsid(getpid())) + sizeof('\0');
			str = malloc(memNeeded);
			snprintf(str, memNeeded, "Child process:\npid = %d;\ngid = %d;\nsid = %d.\n\n", getpid(), getgid(), getsid(getpid()));

			if ((write(logFile, str, strlen(str))) == -1)
			{
				printf("Error with write()\n%s\n", strerror(errno));
				exit(1);
			}
			sleep(1);
		}
	}
	else // Parent
	{
		if ((write(logFile, "Parent process created child process\n", sizeof("Parent process created child process\n") - 1)) == -1)
		{
			printf("Error with write()\n%s\n", strerror(errno));
			exit(1);
		}
	}

	exit(0);
}
