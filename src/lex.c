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
    printf("token: type        = %d\n"
           "       conts       = %s\n"
           "       line        = %d\n"
           "       pos_on_line = %d\n"
           "       length      = %d\n", tok->type, tok->conts, tok->line,
           tok->pos_on_line, tok->length);
}

static struct token lex_identifier(char const *src, unsigned long src_len,
                                   unsigned long start_pos, unsigned long line,
                                   unsigned long pos_on_line)
{
    unsigned long i;
    struct token tok;

    /* get end index of token. */
    for (i = start_pos + 1; i < src_len && (isalnum(src[i]) || src[i] == '_'); ++i);

    /* build token from information. */
    tok.type = TOKEN_TYPE_IDENTIFIER;
    tok.line = line;
    tok.pos_on_line = pos_on_line;
    tok.length = i - start_pos;
    tok.conts = malloc(tok.length + 1);
    memcpy(tok.conts, src + start_pos, tok.length);
    tok.conts[tok.length] = '\0';

    return tok;
}

static struct token lex_string(char const *src, unsigned long src_len,
                               unsigned long start_pos, unsigned long line,
                               unsigned long pos_on_line)
{
    unsigned long i;
    struct token tok;

    /* get end index of token. */
    for (i = start_pos + 1; i < src_len && src[i] != '"'; ++i);

    /* build token from information. */
    tok.type = TOKEN_TYPE_STRING;
    tok.line = line;
    tok.pos_on_line = pos_on_line;
    tok.length = i - start_pos + 1;
    tok.conts = malloc(tok.length + 1);
    memcpy(tok.conts, src + start_pos, tok.length);
    tok.conts[tok.length] = '\0';

    return tok;
}

static struct token lex_number(char const *src, unsigned long src_len,
                               unsigned long start_pos, unsigned long line,
                               unsigned long pos_on_line)
{
    unsigned long i;
    struct token tok;

    /* get end index of token. */
    for (i = start_pos + 1; i < src_len && (isdigit(src[i]) || src[i] == '.'); ++i);

    /* build token from information. */
    tok.type = TOKEN_TYPE_NUMBER;
    tok.line = line;
    tok.pos_on_line = pos_on_line;
    tok.length = i - start_pos;
    tok.conts = malloc(tok.length + 1);
    memcpy(tok.conts, src + start_pos, tok.length);
    tok.conts[tok.length] = '\0';

    return tok;
}

int lex(struct token out_toks[], char const *src, unsigned long src_len)
{
    unsigned long i, line = 1, pos_on_line = 0;
    int next_tok_ind = 0;

    for (i = 0; i < src_len; ++i) {
        struct token new_tok;

        if (src[i] == '\n') {
            ++line;
            pos_on_line = 0;
            continue;
        } else if (src[i] == '"')
            new_tok = lex_string(src, src_len, i, line, pos_on_line);
        else if (isalpha(src[i]))
            new_tok = lex_identifier(src, src_len, i, line, pos_on_line);
        else if (isdigit(src[i]))
            new_tok = lex_number(src, src_len, i, line, pos_on_line);
        else if (isspace(src[i])) {
            ++pos_on_line;
            continue;
        } else {
            printf("unrecognized token at L=%d,c=%d!\n", line, pos_on_line);
            return -1;
        }

        i += new_tok.length - 1;
        pos_on_line += new_tok.length;
        out_toks[next_tok_ind++] = new_tok;
    }

    return 0;
}
