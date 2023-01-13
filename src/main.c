#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "cli.h"
#include "pp.h"
#include "lex.h"
#include "parse.h"

int main(int argc, char const *argv[])
{
    unsigned passed_flags = cli_run(argc, argv);
    FILE *fp = fopen(argv[1], "rb");
    size_t src_len;
    char *src;
    token_list toks = token_list_create();

    if (fp == NULL) {
        printf("reading an invalid file: %s!\n", argv[1]);
        return -1;
    }

    /* read given file into buffer. */
    fseek(fp, 0, SEEK_END);
    src_len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    src = malloc(src_len);
    fread(src, src_len, 1, fp);
    fclose(fp);

    /* preprocess the buffer. */
    src_len = preprocess(src, src_len);

    /* lex the buffer. */
    lex(&toks, src, src_len);
    free(src);

    token_list_destroy(&toks);
    return 0;
}
