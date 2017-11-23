#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    int fd;
    fd_set rfds, wfds;

    if ( (fd = open("/dev/globalfifo3", O_RDWR | O_NONBLOCK)) == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        FD_ZERO(&rfds);
        FD_ZERO(&wfds);
        FD_SET(fd, &rfds);
        FD_SET(fd, &wfds);

        select(fd + 1, &rfds, &wfds, NULL, NULL);

        if (FD_ISSET(fd, &rfds))
            printf("Poll monitor: can be read\n");
        if (FD_ISSET(fd, &wfds))
            printf("Poll monitor: can be written\n");
    }

    exit(EXIT_SUCCESS);
}
