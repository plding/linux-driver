#include "our.h"

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

    if (optind >= argc)
        err_quit("Usage: ./globalfifo_write [-n] string");

    fd = Open("/dev/globalfifo", flag);

    nwritten = Write(fd, argv[optind], strlen(argv[optind]));

    printf("write %zu bytes\n", nwritten);

    exit(EXIT_SUCCESS);
}
