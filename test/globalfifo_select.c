#include "our.h"

int main(void)
{
    int fd;
    fd_set rfds, wfds;

    fd = Open(GLOBALFIFO_FILENAME, O_RDWR|O_NONBLOCK);

    while (1) {
        FD_ZERO(&rfds);
        FD_ZERO(&wfds);
        FD_SET(fd, &rfds);
        FD_SET(fd, &wfds);

        Select(fd + 1, &rfds, &wfds, NULL, NULL);

        if (FD_ISSET(fd, &rfds))
            printf("Poll monitor: can be read\n");
        if (FD_ISSET(fd, &wfds))
            printf("Poll monitor: can be written\n");
    }

    exit(EXIT_SUCCESS);
}
