#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/epoll.h>

int main(void)
{
    int fd, epfd;
    struct epoll_event ev1, ev2;

    if ( (fd = open("/dev/globalfifo3", O_RDWR | O_NONBLOCK)) < 0) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    if ( (epfd = epoll_create(1)) < 0) {
        perror("epoll_create failed");
        exit(EXIT_FAILURE);
    }

    bzero(&ev1, sizeof(struct epoll_event));
    bzero(&ev2, sizeof(struct epoll_event));

    ev1.events = EPOLLIN | EPOLLOUT;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev1) < 0) {
        perror("epoll_ctl - EPOLL_CTL_ADD failed");
        exit(EXIT_FAILURE);
    }

    while (1) {

        if (epoll_wait(epfd, &ev2, 1, -1) < 0) {
            perror("epoll_wait failed");
            exit(EXIT_FAILURE);
        }

        if (ev2.events & EPOLLIN)
            printf("Poll monitor: can be read\n");
        if (ev2.events & EPOLLOUT)
            printf("Poll monitor: can be written\n");
    }
    
    if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev1) < 0) {
        perror("epoll_ctl - EPOLL_CTL_DEL failed");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
