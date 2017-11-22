#include "our.h"

int main(void)
{
    int fd;
    ssize_t nread;
    char buf[255];

    fd = Open("/dev/globalmem", O_RDONLY);

    nread = Read(fd, buf, sizeof(buf));
    buf[nread] = '\0';

    printf("read: %s\n", buf);

    return 0;
}
