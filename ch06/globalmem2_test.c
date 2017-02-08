#include "test.h"

#define PATHNAME1 "/dev/globalmem2_1"
#define PATHNAME2 "/dev/globalmem2_2"

int main(void)
{
    int fd1, fd2;
    char buf[255];

    Mknod(PATHNAME1, S_IRUSR|S_IWUSR|S_IFCHR, makedev(201, 0));
    Mknod(PATHNAME2, S_IRUSR|S_IWUSR|S_IFCHR, makedev(201, 1));

    fd1 = Open(PATHNAME1, O_RDWR);
    fd2 = Open(PATHNAME2, O_RDWR);

    Write(fd1, "hello world1", 12);
    Write(fd2, "hello world2", 12);

    Lseek(fd1, 0, SEEK_SET);
    Lseek(fd2, 0, SEEK_SET);

    Read(fd1, buf, 12);
    if (strncmp(buf, "hello world1", 12) == 0)
        printf("ok\n");
    else
        printf("failed\n");

    Read(fd2, buf, 12);
    if (strncmp(buf, "hello world2", 12) == 0)
        printf("ok\n");
    else
        printf("failed\n");

    Unlink(PATHNAME1);
    Unlink(PATHNAME2);
    
    exit(0);
}
