#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

#define BUFFERLENGTH (1024)

struct datum 
{
    char str[BUFFERLENGTH];
    time_t time;
    int pid;
};

int main(int argc, char** argv) 
{
    char buf[BUFFERLENGTH];
    struct datum* datum_test;
    int fd;

    fd = shm_open("/kv9102", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd == -1) 
        exit(1);
    if (ftruncate(fd, sizeof(struct datum)) == -1)
        exit(1);
    if ((datum_test = mmap(NULL, sizeof(struct datum), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
        exit(1);

    while (1)
    {
        printf("Enter value: ");
        fgets(buf, sizeof buf, stdin);

        if (msync(datum_test, sizeof(struct datum), MS_SYNC) == -1)
            exit(1);

        printf("pid = %d\ntime = %ld\nstr = %s\n\n", datum_test->pid, datum_test->time, datum_test->str);

        datum_test->pid = getpid();
        datum_test->time = time(NULL);

        strcpy(datum_test->str, buf);
    }

    exit(0);
}
