#ifndef FILE_H
#define FILE_H

#include <stdio.h>

// get an out stream to output
FILE *get_outfile(const char *const prefix, const char *const suffix);

#endif // FILE_H