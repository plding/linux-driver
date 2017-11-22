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
#include <fcntl.h>

#define MAXLINE     1000
#define MEM_CLEAR   0x01

/* error functions */
void err_sys(const char *fmt, ...);
void err_quit(const char *fmt, ...);

/* wrap functions */
void    Close(int fd);
pid_t   Fork(void);
void    Ioctl(int fd, int cmd, ...);
off_t   Lseek(int fd, off_t offset, int whence);
void    Mknod(const char *pathname, mode_t mode, dev_t dev);
int     Open(const char *pathname, int flags, ...);
ssize_t Read(int fd, void *buf, size_t count);
void    Unlink(const char *pathname);
void    Write(int fd, const void *buf, size_t count);

#endif  /* __OUR_H */
