#include "our.h"

int main(void)
{
    int fd, i;
    pid_t pid;
    char buf[0x100000];

    for (i = 0; i < 2; i++) {

        if ( (pid = Fork()) == 0) {
            // in child

            fd = Open(GLOBALMEM_FILENAME, O_WRONLY);

            memset(buf, 'a' + i, sizeof(buf));

            Write(fd, buf, sizeof(buf));

            _exit(EXIT_SUCCESS);
        }

        // continue...
    }

    while (wait(NULL) > 0)
        ;

    fd = Open(GLOBALMEM_FILENAME, O_RDONLY);

    Read(fd, buf, sizeof(buf));

    for (i = 1; i < sizeof(buf); i++) {
        if (buf[i] != buf[0]) {
            printf("concurrency write error! buf[0]: %c, buf[%d]: %c\n", buf[0], i, buf[i]);
            exit(EXIT_FAILURE);
        }
    }

    printf("concurrency write success!\n");
    exit(EXIT_SUCCESS);
}
