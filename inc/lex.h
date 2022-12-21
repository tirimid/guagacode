#ifndef LEX_H__
#define LEX_H__

#include <stddef.h>

enum token_type {
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_NUMBER
};

/* free all allocated memory by calling `destroy_token()`. */
struct token {
    char *conts;
    size_t length;
    unsigned line, column;
    enum token_type type;
};

void destroy_token(struct token *tok);
void print_token(struct token const *tok);
int lex(struct token out_toks[], char const *src, size_t src_len);

#endif
