#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int fd, flag, opt;
    ssize_t nread;
    char buf[100] = { 0 };

    flag = O_RDONLY;

    while ((opt = getopt(argc, argv, "n")) != -1) {

        switch (opt) {

        case 'n':
            flag |= O_NONBLOCK;
            break;
        }
    }

    if ( (fd = open("/dev/globalfifo", flag)) == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    if ( (nread = read(fd, buf, sizeof(buf))) == -1) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }

    printf("read %zu bytes, buf: %s\n", nread, buf);

    exit(EXIT_SUCCESS);
}
