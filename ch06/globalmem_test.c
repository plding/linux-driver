#include "test.h"

int main(void)
{
    const char *pathname = "/dev/globalmem";
    char buf[255];
    int fd;

    Mknod(pathname, S_IRUSR|S_IWUSR, makedev(200, 0));

    fd = Open(pathname, O_RDWR);

    Write(fd, "hello world", 11);

    Lseek(fd, 0, SEEK_SET);

    Read(fd, buf, 11);

    if (strncmp(buf, "hello world", 11) == 0)
        printf("ok\n");
    else
        printf("failed\n");

    Unlink(pathname);
    exit(0);
}
