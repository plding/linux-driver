#include "our.h"

void
Close(int fd)
{
    if (close(fd) == -1)
        err_sys("close");
}

pid_t
Fork(void)
{
    pid_t pid;

    if ( (pid = fork()) == -1)
        err_sys("fork");

    return pid;
}

off_t
Lseek(int fd, off_t offset, int whence)
{
    off_t off;
    
    if ( (off = lseek(fd, offset, whence)) == -1)
        err_sys("lseek");

    return off;
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

ssize_t
Read(int fd, void *buf, size_t count)
{
    ssize_t nread;

    if ( (nread = read(fd, buf, count)) == -1)
        err_sys("read");

    return nread;
}

void
Unlink(const char *pathname)
{
    if (unlink(pathname) == -1)
        err_sys("unlink");
}

void
Write(int fd, const void *buf, size_t count)
{
    if (write(fd, buf, count) != (ssize_t) count)
        err_sys("write");
}
