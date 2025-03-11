#ifndef OUTPUT_H
#define OUTPUT_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

void outputToStdOut(const char *buffer);

void assertWithMessage(bool condition, const char *message);

#endif