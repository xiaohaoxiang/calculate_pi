#include "file.h"
#include <stdlib.h>
#include <string.h>

FILE *get_outfile(const char *const prefix, const char *const suffix)
{
    const size_t max_filename_len = strlen(prefix) + strlen(suffix) + 16;
    char *filename = (char *)malloc(max_filename_len * sizeof(char));
    strcpy(filename, "data/");
    long num = 0;
    snprintf(filename + 5, max_filename_len - 5, "%s%ld%s", prefix, num++, suffix);
    FILE *fp = fopen(filename, "r");
    while (fp != NULL)
    {
        snprintf(filename + 5, max_filename_len - 5, "%s%ld%s", prefix, num++, suffix);
        fp = freopen(filename, "r", fp);
    }
    return fopen(filename, "a");
}