#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(void)
{
    int fd, i;
    pid_t pid;
    char buf[0x100000];

    for (i = 0; i < 2; i++) {

        if ( (pid = fork()) == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);

        } else if (pid == 0) {
            // in child

            if ( (fd = open("/dev/globalmem", O_WRONLY)) == -1) {
                perror("open failed");
                exit(EXIT_FAILURE);
            }

            memset(buf, 'a' + i, sizeof(buf));

            (void) write(fd, buf, sizeof(buf));

            _exit(EXIT_SUCCESS);
        }

        // continue...
    }

    while (wait(NULL) > 0)
        ;

    if ( (fd = open("/dev/globalmem", O_RDONLY)) == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    (void) read(fd, buf, sizeof(buf));

    for (i = 1; i < sizeof(buf); i++) {
        if (buf[i] != buf[0]) {
            printf("concurrency write error! buf[0]: %c, buf[%d]: %c\n", buf[0], i, buf[i]);
            exit(EXIT_FAILURE);
        }
    }

    printf("concurrency write success!\n");
    exit(EXIT_SUCCESS);
}
