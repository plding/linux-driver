#include "test.h"

#define PATHNAME "/dev/globalfifo"

int main(void)
{
    int fd, nready;
    fd_set rfds, wfds;
    ssize_t nread;
    // char buf[MAXLINE], cached[MAXLINE], *ptr;

    Mknod(PATHNAME, S_IRUSR|S_IWUSR|S_IFCHR, makedev(200, 0));

    fd = Open(PATHNAME, O_RDWR);

    // ptr = cached;

    // for ( ; ; ) {
    //     FD_ZERO(&rfds);
    //     FD_ZERO(&wfds);

    //     FD_SET(fd, &rfds);
    //     FD_SET(STDIN_FILENO, &rfds);

    //     FD_SET(fd, &wfds);

    //     nready = Select(fd + 1, &rfds, &wfds, NULL, NULL);

    //     if (FD_ISSET(fd, &rfds)) {
    //         if ( (nread = Read(fd, buf, sizeof(buf))) > 0)
    //             Write(STDOUT_FILENO, buf, nread);
    //         else if (nread == 0)
    //             printf("quit\n");

    //         if (--nready <= 0)
    //             continue;
    //     }

    //     if (FD_ISSET(STDIN_FILENO, &rfds)) {
    //         if ( (nread = Read(STDIN_FILENO, buf, sizeof(buf))) > 0)
    //             memcpy();

    //         if (--nready <= 0)
    //             continue;
    //     }

    //     if (FD_ISSET(fd, &wfds)) {
    //         size_t len = ptr - cache;

    //         nwritten = Write(fd, cache, len);
    //         memcpy(cache, cache + nwritten, len - nwritten);
    //         ptr = cache + (len - nwritten);

    //         if (--nready <= 0)
    //             continue;
    //     }
    // }
    
    
    Unlink(PATHNAME);

    exit(0);
}
