#include "test.h"

int main(void)
{
    const char *pathname1 = "/dev/globalmem2_1";
    const char *pathname2 = "/dev/globalmem2_2";
    char buf[255];
    int fd1, fd2;

    Mknod(pathname1, S_IRUSR|S_IWUSR, makedev(200, 0));
    Mknod(pathname2, S_IRUSR|S_IWUSR, makedev(200, 1));

    fd1 = Open(pathname1, O_RDWR);
    fd2 = Open(pathname2, O_RDWR);

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

    Unlink(pathname1);
    Unlink(pathname2);
    
    exit(0);
}
