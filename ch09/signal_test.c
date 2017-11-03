#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

static void input_handler(int signo)
{
    char buf[100];
    ssize_t nread;

    if ( (nread = read(STDIN_FILENO, buf, sizeof(buf))) < 0) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }

    buf[nread] = '\0';

    printf("input available: %s", buf);
}

int main(void)
{
    int flags;

    if (fcntl(STDIN_FILENO, F_SETOWN, getpid()) < 0) {
        perror("fcntl - F_SETOWN failed");
        exit(EXIT_FAILURE);
    }

    if ( (flags = fcntl(STDIN_FILENO, F_GETFL)) < 0) {
        perror("fcntl - F_GETFL failed");
        exit(EXIT_FAILURE);
    }

    if (fcntl(STDIN_FILENO, F_SETFL, flags | FASYNC) < 0) {
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
