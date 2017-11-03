#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

static void input_handler(int signo)
{
    printf("receive a signal from globalfifo\n");
}

int main(void)
{
    int fd, flags;

    if ( (fd = open("/dev/globalfifo", O_RDONLY)) < 0) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    if (fcntl(fd, F_SETOWN, getpid()) < 0) {
        perror("fcntl - F_SETOWN failed");
        exit(EXIT_FAILURE);
    }

    if ( (flags = fcntl(fd, F_GETFL)) < 0) {
        perror("fcntl - F_GETFL failed");
        exit(EXIT_FAILURE);
    }

    if (fcntl(fd, F_SETFL, flags | FASYNC) < 0) {
        perror("fcntl - F_SETFL failed");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGIO, input_handler) < 0) {
        perror("signal failed");
        exit(EXIT_FAILURE);
    }

    while (1)
        ;
}
