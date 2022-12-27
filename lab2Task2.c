#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFFERLENGTH (1024)
#define CHMOD (0644)

int main(int argc, char** argv, char** envp)
{
	pid_t pid;
	int log;

	if ((log = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, CHMOD)) == -1)
		exit(1);
	if ((write(log, "Begin parent proc\n", sizeof("Begin parent proc\n") - 1)) == -1)
		exit(1);
	if ((pid = fork()) < 0)
		exit(1);

	if (pid != 0)
	{
		if ((write(log, "Child proc was created by parent proc\n", sizeof("Child proc was created by parent proc\n") - 1)) == -1)
			exit(1);
	}
	else
	{
		char* path = malloc(BUFFERLENGTH * sizeof * path);
		getcwd(path, BUFFERLENGTH);

		char* text;
		int dev_null;
		size_t memory;

		if (setsid() == -1 || chdir("/") == -1 || close(log) == -1)
			exit(1);
		if ((dev_null = open("/dev/null", O_WRONLY)) == -1)
			exit(1);

		for (size_t i = 0; i < 3; i++)
			if (dup2(dev_null, i) == -1)
				exit(1);

		strcat(path, "/log.txt");

		if ((log = open(path, O_WRONLY | O_APPEND, CHMOD)) == -1)
			exit(1);
		if ((write(log, "Created child proc\n", sizeof("Created child proc\n") - 1)) == -1)
			exit(1);

		while (1)
		{
			memory = snprintf(NULL, 0, "Child proc\npid - %d\nsid - %d\ngid - %d\n\n", getpid(), getsid(getpid()), getgid()) + sizeof('\0');
			text = malloc(memory);
			snprintf(text, memory, "Child proc\npid - %d\nsid - %d\ngid - %d\n\n", getpid(), getsid(getpid()), getgid());

			if ((write(log, text, strlen(text))) == -1)
				exit(1);

			sleep(2);
		}
	}

	exit(0);
}
