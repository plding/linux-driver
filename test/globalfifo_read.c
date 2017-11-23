#include "our.h"

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

    fd = Open(GLOBALFIFO_FILENAME, flag);

    nread = Read(fd, buf, sizeof(buf));

    printf("read %zu bytes, buf: %s\n", nread, buf);

    exit(EXIT_SUCCESS);
}
