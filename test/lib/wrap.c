#include "our.h"

char *
Calloc(size_t size)
{
    char *mem = Malloc(size);

    if (!mem)
        memset(mem, 0, size);

    return mem;
}

void
Close(int fd)
{
    if (close(fd) == -1)
        err_sys("close");
}

int
Epoll_create(int size)
{
    int epfd;

    if ( (epfd = epoll_create(size)) < 0)
        err_sys("epoll_create");

    return epfd;
}

void
Epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
{
    if (epoll_ctl(epfd, op, fd, event) < 0)
        err_sys("epoll_ctl");
}

void
Epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
{
    if (epoll_wait(epfd, events, maxevents, timeout) < 0)
        err_sys("epoll_wait");
}

pid_t
Fork(void)
{
    pid_t pid;

    if ( (pid = fork()) == -1)
        err_sys("fork");

    return pid;
}

void
Ioctl(int fd, int cmd, ...)
{
    va_list ap;
    unsigned long arg;

    va_start(ap, cmd);
    arg = va_arg(ap, unsigned long);
    va_end(ap);

    if (ioctl(fd, cmd, arg) < 0)
        err_sys("ioctl");
}

off_t
Lseek(int fd, off_t offset, int whence)
{
    off_t off;
    
    if ( (off = lseek(fd, offset, whence)) == -1)
        err_sys("lseek");

    return off;
}

char
*Malloc(size_t size)
{
    char *mem;

    if (!(mem = malloc(size)))
        err_sys("malloc");

    return mem;
}

void
Mknod(const char *pathname, mode_t mode, dev_t dev)
{
    if (mknod(pathname, mode, dev) == -1)
        err_sys("mknod");
}

int
Open(const char *pathname, int flags, ...)
{
    va_list ap;
    mode_t mode = -1;
    int fd;

    va_start(ap, flags);
    mode = va_arg(ap, mode_t);
    va_end(ap);

    if (mode)
        fd = open(pathname, flags, mode);
    else
        fd = open(pathname, flags);

    if (fd == -1)
        err_sys("open");

    return fd;
}

int
Poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    int nready;

    if ( (nready = poll(fds, nfds, timeout)) < 0)
        err_sys("poll");

    return nready;
}

ssize_t
Pwrite(int fd, const void *buf, size_t count, off_t offset)
{
    ssize_t nwritten;

    if ( (nwritten = pwrite(fd, buf, count, offset)) == -1)
        err_sys("pwrite");

    return nwritten;
}

ssize_t
Read(int fd, void *buf, size_t count)
{
    ssize_t nread;

    if ( (nread = read(fd, buf, count)) == -1)
        err_sys("read");

    return nread;
}

int
Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
    int nready;

    if ( (nready = select(nfds, readfds, writefds, exceptfds, timeout)) == -1)
        err_sys("select");

    return nready;
}

void
Unlink(const char *pathname)
{
    if (unlink(pathname) == -1)
        err_sys("unlink");
}

ssize_t
Write(int fd, const void *buf, size_t count)
{
    ssize_t nwritten;

    if ( (nwritten = write(fd, buf, count)) == -1)
        err_sys("write");

    return nwritten;
}
