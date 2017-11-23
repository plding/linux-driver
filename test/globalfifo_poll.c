#include "our.h"

int main(void)
{
    int fd;
    struct pollfd fds[1];

    fd = Open(GLOBALFIFO_FILENAME, O_RDWR | O_NONBLOCK);

    fds[0].fd = fd;
    fds[0].events = POLLRDNORM | POLLWRNORM;

    while (1) {

        Poll(fds, 1, -1);

        if (fds[0].revents & POLLRDNORM)
            printf("Poll monitor: can be read\n");
        if (fds[0].revents & POLLWRNORM)
            printf("Poll monitor: can be written\n");
    }

    exit(EXIT_SUCCESS);
}
