#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    int fd;
    char buf[100] = { 0 };

    if ( (fd = open("/dev/globalmem", O_RDWR)) == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    if (write(fd, "hello world", 11) != 11) {
        perror("write failed");
        exit(EXIT_FAILURE);
    }

    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("lseek failed");
        exit(EXIT_FAILURE);
    }

    if (read(fd, buf, sizeof(buf)) == -1) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }

    printf("buf: %s\n", buf);

    exit(EXIT_SUCCESS);
}
