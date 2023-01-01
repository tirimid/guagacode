#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include "lex.h"

#define ALL_GCC_OUTPUT_BITS (FLAG_BIT_OBJECT | FLAG_BIT_BINARY)
#define ALL_OPTIMIZE_BITS (FLAG_BIT_OPTIMIZE_0 | \
                           FLAG_BIT_OPTIMIZE_1 | \
                           FLAG_BIT_OPTIMIZE_2 | \
                           FLAG_BIT_OPTIMIZE_3)

enum flag_bit {
    FLAG_BIT_OBJECT = 1 << 0,
    FLAG_BIT_BINARY = 1 << 1,
    FLAG_BIT_OPTIMIZE_0 = 1 << 2,
    FLAG_BIT_OPTIMIZE_1 = 1 << 3,
    FLAG_BIT_OPTIMIZE_2 = 1 << 4,
    FLAG_BIT_OPTIMIZE_3 = 1 << 5
};

static char const *valid_flags[] = {
    "-o",
    "-b",
    "-O0",
    "-O1",
    "-O2",
    "-O3"
};

static enum flag_bit const valid_flag_meanings[] = {
    FLAG_BIT_OBJECT,
    FLAG_BIT_BINARY,
    FLAG_BIT_OPTIMIZE_0,
    FLAG_BIT_OPTIMIZE_1,
    FLAG_BIT_OPTIMIZE_2,
    FLAG_BIT_OPTIMIZE_3
};

static bool flag_is_valid(char const *flag)
{
    int i;

    for (i = 0; i < sizeof(valid_flags) / sizeof(valid_flags[0]); ++i) {
        if (strcmp(flag, valid_flags[i]) == 0)
            return true;
    }

    return false;
}

static int count_set_bits(unsigned long val)
{
    int i, cnt = 0;

    for (i = 0; i < sizeof(val) * CHAR_BIT; ++i) {
        if ((val & 1 << i) > 0)
            ++cnt;
    }

    return cnt / 2;
}

int main(int argc, char const *argv[])
{
    int i, file_len;
    char buf[512] = {'\0'};
    FILE *fp;
    struct token toks[7];
    unsigned long passed_flags = 0;
    
    /* print the explanatory message if no arguments are given. */
    if (argc == 1) {
        printf("Guagacode (/ɡwɑ.ɡə.koʊd/) is a programming language made for a"
               " YouTube tutorial.\n"
               "Check the channel \"@tirimid\" on YouTube.\n");
        return 0;
    }

    /* make sure there are enough arguments to actually run the progam. */
    if (argc < 3) {
        printf("incorrect usage!\n"
               "correct usage: `guagacode <source> <output> [flags]`\n");
        return -1;
    }

    /* verify that all passed flags are valid. */
    for (i = 3; i < argc; ++i) {
        if (flag_is_valid(argv[i]))
            continue;
        
        printf("invalid flag: %s!\n"
               "valid flags are:\n", argv[i]);
        for (i = 0; i < sizeof(valid_flags) / sizeof(valid_flags[0]); ++i)
            printf("    %s\n", valid_flags[i]);
        
        return -1;
    }

    /* extract all passed flags into an integer as bits. */
    for (i = 3; i < argc; ++i) {
        int j;

        for (j = 0; j < sizeof(valid_flags) / sizeof(valid_flags[0]); ++j) {
            if (strcmp(argv[i], valid_flags[j]) == 0)
                passed_flags |= valid_flag_meanings[j];
        }
    }

    /* check that flags are mutually valid. */
    if ((passed_flags & ALL_OPTIMIZE_BITS) > 0
        && (passed_flags & ALL_GCC_OUTPUT_BITS) == 0) {
        printf("optimization flags require GCC output!\n");
        return -1;
    }

    if (count_set_bits(passed_flags & ALL_OPTIMIZE_BITS) > 1) {
        printf("only one optimization level can be enabled!\n");
        return -1;
    }

    if (count_set_bits(passed_flags & ALL_GCC_OUTPUT_BITS) > 1) {
        printf("only one GCC output format can be enabled!\n");
        return -1;
    }

    /* test the functionality of the lexer. */
    fp = fopen("design/lex_test.ggc", "rb");
    fseek(fp, 0, SEEK_END);
    file_len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    fread(buf, file_len, 1, fp);
    fclose(fp);

    if (lex(toks, buf, file_len) == -1)
        return -1;

    for (i = 0; i < 7; ++i) {
        print_token(&toks[i]);
        printf("\n");
        destroy_token(&toks[i]);
    }
    
    return 0;
}
