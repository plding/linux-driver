#include "our.h"

int main(void)
{
    int fd;
    char buf[255] = { 0 };

    fd = Open("/dev/globalmem", O_RDONLY);

    Read(fd, buf, sizeof(buf));

    printf("read: '%s'\n", buf);

    return 0;
}
