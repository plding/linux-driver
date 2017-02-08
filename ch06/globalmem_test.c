#include "test.h"

#define PATHNAME "/dev/globalmem"

int main(void)
{
    int fd;
    char buf[255];

    Mknod(PATHNAME, S_IRUSR|S_IWUSR|S_IFCHR, makedev(200, 0));

    fd = Open(PATHNAME, O_RDWR);

    Write(fd, "hello world", 11);

    Lseek(fd, 0, SEEK_SET);

    Read(fd, buf, 11);

    if (strncmp(buf, "hello world", 11) == 0)
        printf("ok\n");
    else
        printf("failed\n");

    Unlink(PATHNAME);
    exit(0);
}
