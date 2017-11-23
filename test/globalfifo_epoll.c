#include "our.h"

int main(void)
{
    int fd, epfd;
    struct epoll_event ev1, ev2;

    fd = Open(GLOBALFIFO_FILENAME, O_RDWR | O_NONBLOCK);

    epfd = Epoll_create(1);

    bzero(&ev1, sizeof(struct epoll_event));
    bzero(&ev2, sizeof(struct epoll_event));

    ev1.events = EPOLLIN | EPOLLOUT;

    Epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev1);

    while (1) {

        Epoll_wait(epfd, &ev2, 1, -1);

        if (ev2.events & EPOLLIN)
            printf("Epoll monitor: can be read\n");
        if (ev2.events & EPOLLOUT)
            printf("Epoll monitor: can be written\n");
    }
    
    Epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev1);

    exit(EXIT_SUCCESS);
}
