#include "pp.h"

#include <string.h>
#include <stdbool.h>

size_t preprocess(char *src, size_t src_len)
{
    size_t i;
    bool in_string = false;

    /* remove `//` comments from the source string. */
    for (i = 0; i < src_len - 1; ++i) {
        if (src[i] == '"')
            in_string = !in_string;
        else if (src[i] == '/' && src[i + 1] == '/' && !in_string) {
            size_t end;
            
            for (end = i; src[end] != '\n' && end < src_len; ++end);
            memmove(src + i, src + end, src_len - end);
            src_len -= end - i;
        }
    }

    return src_len;
}
