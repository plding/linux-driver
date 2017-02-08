#include "test.h"

void
err_sys(const char *fmt, ...)
{
    va_list ap;
    char buf[255] = { 0 };

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    fflush(stdout);
    fprintf(stderr, "%s: %s\n", buf, strerror(errno));
    fflush(stderr);

    exit(-1);
}
