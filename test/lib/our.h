#ifndef __OUR_H
#define __OUR_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <poll.h>

#define MAXLINE     1000
#define MEM_CLEAR   0x01

#define GLOBALMEM_FILENAME  "/dev/globalmem"
#define GLOBALFIFO_FILENAME "/dev/globalfifo"

/* error functions */
void err_sys(const char *fmt, ...);
void err_quit(const char *fmt, ...);

/* wrap functions */
char   *Calloc(size_t size);
void    Close(int fd);
int     Epoll_create(int size);
void    Epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
void    Epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
pid_t   Fork(void);
void    Ioctl(int fd, int cmd, ...);
off_t   Lseek(int fd, off_t offset, int whence);
char   *Malloc(size_t size);
void    Mknod(const char *pathname, mode_t mode, dev_t dev);
int     Open(const char *pathname, int flags, ...);
int     Poll(struct pollfd *fds, nfds_t nfds, int timeout);
ssize_t Pwrite(int fd, const void *buf, size_t count, off_t offset);
ssize_t Read(int fd, void *buf, size_t count);
int     Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
void    Unlink(const char *pathname);
ssize_t Write(int fd, const void *buf, size_t count);

#endif  /* __OUR_H */
