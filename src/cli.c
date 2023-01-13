#include "cli.h"

#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

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

unsigned cli_run(int argc, char const *argv[])
{
    unsigned passed_flags = 0;
    int i;
    
    /* print the explanatory message if no arguments are given. */
    if (argc == 1) {
        printf("Guagacode (/ɡwɑ.ɡə.koʊd/) is a programming language made for a"
               " YouTube tutorial.\n"
               "Check the channel \"@tirimid\" on YouTube.\n");
        exit(0);
    }

    /* make sure there are enough arguments to actually run the program. */
    if (argc < 3) {
        printf("incorrect usage!\n"
               "correct usage: `guagacode <source> <output> [flags]`\n");
        exit(-1);
    }

    /* verify that all passed flags are valid. */
    for (i = 3; i < argc; ++i) {
        if (flag_is_valid(argv[i]))
            continue;
        
        printf("invalid flag: %s!\n"
               "valid flags are:\n", argv[i]);
        for (i = 0; i < sizeof(valid_flags) / sizeof(valid_flags[0]); ++i)
            printf("    %s\n", valid_flags[i]);
        
        exit(-1);
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
        exit(-1);
    }

    if (count_set_bits(passed_flags & ALL_OPTIMIZE_BITS) > 1) {
        printf("only one optimization level can be enabled!\n");
        exit(-1);
    }

    if (count_set_bits(passed_flags & ALL_GCC_OUTPUT_BITS) > 1) {
        printf("only one GCC output format can be enabled!\n");
        exit(-1);
    }

    return passed_flags;
}
