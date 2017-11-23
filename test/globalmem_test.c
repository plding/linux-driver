#include "our.h"

#define FILENAME    "/dev/globalmem"
#define MAX_SIZE    255

static char *read_globalmem()
{
    int fd;
    char *buf = Calloc(MAX_SIZE);

    fd = Open(FILENAME, O_RDONLY);
    Read(fd, buf, MAX_SIZE);

    return buf;
}

int main(void)
{
    int fd, i;
    const char *words[] = { "hello", " ", "world", "!" };
    ssize_t nwritten;

    fd = Open(FILENAME, O_WRONLY);

    printf("test write\n");
    Ioctl(fd, MEM_CLEAR);

    for (i = 0; i < 4; i++) {
        nwritten = Write(fd, words[i], strlen(words[i]));
        printf("write '%s' to globalmem, success written %zu bytes, and now globalmem is '%s'\n", words[i], nwritten, read_globalmem());
    }
    printf("\n");

    printf("test offset write, use lseek\n");
    Ioctl(fd, MEM_CLEAR);

    Lseek(fd, 0, SEEK_SET);
    Write(fd, "hello", 5);
    printf("write at #0, globalmem is '%s'\n", read_globalmem());

    Lseek(fd, 2, SEEK_SET);
    Write(fd, "hello", 5);
    printf("write at #2, globalmem is '%s'\n", read_globalmem());

    Lseek(fd, -2, SEEK_CUR);
    Write(fd, "hello", 5);
    printf("write at #5, globalmem is '%s'\n", read_globalmem());

    Lseek(fd, -2, SEEK_END);
    Write(fd, "hello", 8);
    printf("write at #8, globalmem is '%s'\n", read_globalmem());

    printf("\n");

    printf("test offset write, use pwrite\n");
    Ioctl(fd, MEM_CLEAR);

    Pwrite(fd, "hello", 5, 0);
    printf("write at #0, globalmem is '%s'\n", read_globalmem());

    Pwrite(fd, "hello", 5, 2);
    printf("write at #2, globalmem is '%s'\n", read_globalmem());

    Pwrite(fd, "hello", 5, 5);
    printf("write at #5, globalmem is '%s'\n", read_globalmem());

    Pwrite(fd, "hello", 5, 8);
    printf("write at #8, globalmem is '%s'\n", read_globalmem());

    printf("\n");

    return 0;
}

