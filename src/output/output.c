#include "output.h"

void outputToStdOut(const char *buffer) { puts(buffer); }

void assertWithMessage(const bool condition, const char *message)
{
    if (!condition) {
        fprintf(stderr, "%s\n", message);
        assert(condition);
    }
}
