#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv)
{
    int fd, flag, opt;
    ssize_t nwritten;

    flag = O_WRONLY;

    while ((opt = getopt(argc, argv, "n")) != -1) {

        switch (opt) {

        case 'n':
            flag |= O_NONBLOCK;
            break;
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Usage: ./globalfifo_write [-n] string\n");
        exit(EXIT_FAILURE);
    } 

    if ( (fd = open("/dev/globalfifo", flag)) == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    if ( (nwritten = write(fd, argv[optind], strlen(argv[optind]))) == -1) {
        perror("write failed");
        exit(EXIT_FAILURE);
    }

    printf("write %zu bytes\n", nwritten);

    exit(EXIT_SUCCESS);
}
