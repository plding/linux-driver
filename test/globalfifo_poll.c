#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

int main(void)
{
    int fd;
    struct pollfd fds[1];

    if ( (fd = open("/dev/globalfifo3", O_RDWR | O_NONBLOCK)) == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    fds[0].fd = fd;
    fds[0].events = POLLRDNORM | POLLWRNORM;

    while (1) {

        if (poll(fds, 1, -1) == -1) {
            perror("poll failed");
            exit(EXIT_FAILURE);
        }

        if (fds[0].revents & POLLRDNORM)
            printf("Poll monitor: can be read\n");
        if (fds[0].revents & POLLWRNORM)
            printf("Poll monitor: can be written\n");
    }

    exit(EXIT_SUCCESS);
}
