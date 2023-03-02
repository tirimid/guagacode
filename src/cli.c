#include "cli.h"

#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

struct flag_lut_entry {
    char const *flag;
    enum flag_bit bit;
};

static struct flag_lut_entry const flags[] = {
    {"-o", FLAG_BIT_OBJECT},
    {"-b", FLAG_BIT_BINARY},
    {"-O0", FLAG_BIT_OPTIMIZE_0},
    {"-O1", FLAG_BIT_OPTIMIZE_1},
    {"-O2", FLAG_BIT_OPTIMIZE_2},
    {"-O3", FLAG_BIT_OPTIMIZE_3}
};

static bool flag_is_valid(char const *flag)
{
    int i;

    for (i = 0; i < sizeof(flags) / sizeof(flags[0]); ++i) {
        if (strcmp(flag, flags[i].flag) == 0)
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
        printf("Guagacode (/ɡwɑ.ɡə.koʊd/) is a programming language made for");
        printf(" a YouTube tutorial.\n");
        printf("Check the channel \"@tirimid\" on YouTube.\n");
        exit(0);
    }

    /* make sure there are enough arguments to actually run the program. */
    if (argc < 3) {
        printf("incorrect usage!\n");
        printf("correct usage: `guagacode <source> <output> [flags]`\n");
        exit(-1);
    }

    /* verify that all passed flags are valid. */
    for (i = 3; i < argc; ++i) {
        if (flag_is_valid(argv[i]))
            continue;
        
        printf("invalid flag: %s!\n");
        printf("valid flags are:\n", argv[i]);
        for (i = 0; i < sizeof(flags) / sizeof(flags[0]); ++i)
            printf("    %s\n", flags[i].flag);
        
        exit(-1);
    }

    /* extract all passed flags into an integer as bits. */
    for (i = 3; i < argc; ++i) {
        int j;

        for (j = 0; j < sizeof(flags) / sizeof(flags[0]); ++j) {
            if (strcmp(argv[i], flags[j].flag) == 0)
                passed_flags |= flags[j].bit;
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
