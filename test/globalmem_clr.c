#include "our.h"

int main(void)
{
    int fd;

    fd = Open("/dev/globalmem", O_RDWR);

    Ioctl(fd, MEM_CLEAR);

    return 0;
}
