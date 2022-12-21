#include "lex.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

void destroy_token(struct token *tok)
{
    free(tok->conts);
}

void print_token(struct token const *tok)
{
    printf("token: type   = %d\n"
           "       conts  = %s\n"
           "       line   = %d\n"
           "       column = %d\n"
           "       length = %d\n", tok->type, tok->conts, tok->line,
           tok->column, tok->length);
}

static struct token build_token(char const *src, size_t start, size_t end,
                                unsigned line, unsigned column,
                                enum token_type type)
{
    struct token tok;

    tok.type = type;
    tok.line = line;
    tok.column = column;
    tok.length = end - start;
    tok.conts = malloc(tok.length + 1);
    memcpy(tok.conts, src + start, tok.length);
    tok.conts[tok.length] = '\0';

    return tok;
}

static struct token lex_identifier(char const *src, size_t src_len,
                                   size_t start, unsigned line, unsigned column)
{
    size_t end = start + 1;
    
    for (; end < src_len && (isalnum(src[end]) || src[end] == '_'); ++end);
    return build_token(src, start, end, line, column, TOKEN_TYPE_IDENTIFIER);
}

static struct token lex_string(char const *src, size_t src_len, size_t start,
                               unsigned line, unsigned column)
{
    size_t end = start + 1;

    for (; end < src_len && src[end] != '"'; ++end);
    return build_token(src, start, end + 1, line, column, TOKEN_TYPE_STRING);
}

static struct token lex_number(char const *src, size_t src_len, size_t start,
                               unsigned line, unsigned column)
{
    size_t end = start + 1;
    
    for (; end < src_len && (isdigit(src[end]) || src[end] == '.'); ++end);
    return build_token(src, start, end, line, column, TOKEN_TYPE_NUMBER);
}

int lex(struct token out_toks[], char const *src, size_t src_len)
{
    unsigned line = 1, column = 1;
    size_t next_tok_ind = 0, i = 0;

    while (i < src_len) {
        struct token new_tok;

        if (src[i] == '\n') {
            column = 1;
            ++line;
            ++i;
            continue;
        } else if (src[i] == '"')
            new_tok = lex_string(src, src_len, i, line, column);
        else if (isalpha(src[i]))
            new_tok = lex_identifier(src, src_len, i, line, column);
        else if (isdigit(src[i]))
            new_tok = lex_number(src, src_len, i, line, column);
        else if (isspace(src[i])) {
            ++column;
            ++i;
            continue;
        } else {
            printf("unrecognized token at l=%d,c=%d!\n", line, column);
            return -1;
        }

        i += new_tok.length;
        column += new_tok.length;
        out_toks[next_tok_ind++] = new_tok;
    }

    return 0;
}
