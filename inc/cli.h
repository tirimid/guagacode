#ifndef CLI_H__
#define CLI_H__

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

unsigned cli_run(int argc, char const *argv[]);

#endif
