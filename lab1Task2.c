#define BUFFERLENGTH (1024)

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv, char** envp)
{
	if (argc != 2)
	{
		printf("Enter one argument");
		exit(1);
	}

    char buffer[BUFFERLENGTH];
    fd_set readfds;
	ssize_t readLen;

	struct timeval timeOut;
	timeOut.tv_usec = 0;

	while (1)
	{
		FD_ZERO(&readfds);
		FD_SET(STDIN_FILENO, &readfds);

		timeOut.tv_sec = 5;

		switch (select(1, &readfds, NULL, NULL, &timeOut))
		{
            case 0:
                printf("TimeOut Error\n");
                break;
            case -1:
                printf("Select Error\n");
                exit(1);
            default:
                if ((readLen = read(STDIN_FILENO, buffer, BUFFERLENGTH)) > 0)
                {
                    ssize_t argLen = strlen(argv[1]);
                    if (write(STDOUT_FILENO, buffer, readLen - 1) != readLen - 1)
                    {
                        printf("Write Buffer Error\n");
                        exit(1);
                    }
                    if (write(STDOUT_FILENO, " - ", 4) != 4)
                    {
                        printf("Write Error\n");
                        exit(1);
                    }
                    if (write(STDOUT_FILENO, argv[1], argLen) != argLen)
                    {
                        printf("Write Argument Error\n");
                        exit(1);
                    }
                    if (write(STDOUT_FILENO, "\n", 2) != 2)
                    {
                        printf("Write Error\n");
                        exit(1);
                    }
                }
                else
                {
                    printf("Read Error\n");
                    exit(1);
                }
                break;
        }
    }
	exit(0);
}
