#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* error functions */
void err_sys(const char *fmt, ...);

/* wrap functions */
void    Close(int fd);
off_t   Lseek(int fd, off_t offset, int whence);
void    Mknod(const char *pathname, mode_t mode, dev_t dev);
int     Open(const char *pathname, int flags, ...);
ssize_t Read(int fd, void *buf, size_t count);
void    Unlink(const char *pathname);
void    Write(int fd, const void *buf, size_t count);
