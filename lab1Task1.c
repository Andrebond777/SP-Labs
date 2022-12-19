#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFERLENGTH (512)
#define CHMOD (0644)

struct pair_int_size_t
{
	int readLength;
	size_t writeLength;
};

typedef struct pair_int_size_t Pair_int_size_t;

void serialIOFunc(const char* fName1, const char* fName2, Pair_int_size_t* result)
{
	ssize_t readLength;
	ssize_t writeLength = 0;
	int file1;
	int file2;
	char buffer[BUFFERLENGTH];

	file1 = open(fName1, O_RDONLY);
	if (file1 == -1)
	{
		result->readLength = -1;
		result->writeLength = writeLength;
		return;
	}

	file2 = open(fName2, O_WRONLY | O_CREAT | O_TRUNC, CHMOD);
	if (file2 == -1)
	{
		close(file1);

		result->readLength = -2;
		result->writeLength = writeLength;
		return;
	}

	while ((readLength = read(file1, buffer, BUFFERLENGTH)) > 0)
	{
		writeLength += readLength;

		for (ssize_t i = 0; i < readLength; i++)
			if (buffer[i] >= 'a' && buffer[i] <= 'z')
				buffer[i] += 'A' - 'a';


		if (write(file2, buffer, readLength) != readLength)
		{
			close(file1);
			close(file2);

			result->readLength = -3;
			result->writeLength = writeLength;
			return;
		}
	}

	close(file1);
	close(file2);

	if(readLength == -1)
	{
		result->readLength = -4;
		result->writeLength = writeLength;
	}
	else
	{
		result->readLength = 0;
		result->writeLength = writeLength;
	}
	return;
}

int main(int argc, char** argv, char** envp)
{
	if (argc != 3)
	{
		printf("Enter two arguments");
		exit(1);
	}

    Pair_int_size_t result;
	serialIOFunc(argv[1], argv[2], &result);

	switch (result.readLength)
	{
		case 0:
			printf("Written bytes length = %li\n", result.writeLength);
			break;
		case -1:
			printf("Error opening file1\n");
			break;
		case -2:
			printf("Error opening file2\n");
			break;
		case -3:
			printf("Error while writing\n");
			break;
		case -4:
			printf("Error while reading\n");
			break;
		default:
			break;
	}

	printf("%s\n", strerror(errno));
	exit(1);
}
