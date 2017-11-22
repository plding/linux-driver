#include "our.h"

int main(int argc, char **argv)
{
    int fd, offset;
    ssize_t nwritten;

    if (argc != 3)
        err_quit("usage: ./globalmem_write <offset> <string>");

    if ( (offset = atoi(argv[1])) < 0)
        err_quit("offset must be a integer!\n");

    fd = Open("/dev/globalmem", O_WRONLY);

    Lseek(fd, offset, SEEK_SET);

    if ( (nwritten = write(fd, argv[2], strlen(argv[2]))) > 0) {
        printf("written %zu byte%s from %i\n", nwritten, nwritten > 1 ? "s" : "", offset);
    }

    return 0;
}
