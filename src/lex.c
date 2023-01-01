#include "lex.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

static char const *keywords[] = {
    "function",
    "returns",
    "entry_point",
    "var",
    "raw",
    "int8",
    "int16",
    "int32",
    "int64",
    "uint8",
    "uint16",
    "uint32",
    "uint64",
    "bool",
    "true",
    "false"
};

static enum token_type const keyword_meanings[] = {
    TOKEN_TYPE_KEYWORD_FUNCTION,
    TOKEN_TYPE_KEYWORD_RETURNS,
    TOKEN_TYPE_KEYWORD_ENTRY_POINT,
    TOKEN_TYPE_KEYWORD_VAR,
    TOKEN_TYPE_KEYWORD_RAW,
    TOKEN_TYPE_KEYWORD_INT8,
    TOKEN_TYPE_KEYWORD_INT16,
    TOKEN_TYPE_KEYWORD_INT32,
    TOKEN_TYPE_KEYWORD_INT64,
    TOKEN_TYPE_KEYWORD_UINT8,
    TOKEN_TYPE_KEYWORD_UINT16,
    TOKEN_TYPE_KEYWORD_UINT32,
    TOKEN_TYPE_KEYWORD_UINT64,
    TOKEN_TYPE_KEYWORD_BOOL,
    TOKEN_TYPE_KEYWORD_TRUE,
    TOKEN_TYPE_KEYWORD_FALSE
};

static char const special_chars[] = "{}()<>,!*/%+-=&|^;";

static enum token_type const special_char_meanings[] = {
    TOKEN_TYPE_BRACE_LEFT,
    TOKEN_TYPE_BRACE_RIGHT,
    TOKEN_TYPE_PAREN_LEFT,
    TOKEN_TYPE_PAREN_RIGHT,
    TOKEN_TYPE_ANGLE_LEFT,
    TOKEN_TYPE_ANGLE_RIGHT,
    TOKEN_TYPE_COMMA,
    TOKEN_TYPE_EXCLAMATION,
    TOKEN_TYPE_ASTERISK,
    TOKEN_TYPE_SLASH,
    TOKEN_TYPE_PERCENTAGE,
    TOKEN_TYPE_PLUS,
    TOKEN_TYPE_MINUS,
    TOKEN_TYPE_EQUALS,
    TOKEN_TYPE_AMPERSAND,
    TOKEN_TYPE_PIPE,
    TOKEN_TYPE_CARET,
    TOKEN_TYPE_SEMICOLON,
};

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

static struct token lex_special(char const *src, size_t src_len, size_t start,
                                unsigned line, unsigned column)
{
    size_t end = start + 1;
    size_t special_ind = strchr(special_chars, src[start]) - special_chars;
    enum token_type type = special_char_meanings[special_ind];
    
    return build_token(src, start, end, line, column, type);
}

/*
 * after the first pass, every extracted identifier token might actually be a
 * keyword; this function will find the actual type of the token based on its
 * textual contents.
 */
static enum token_type identifier_type(char const *conts)
{
    size_t i;

    for (i = 0; i < sizeof(keywords) / sizeof(keywords[0]); ++i) {
        if (strcmp(conts, keywords[i]) == 0)
            return keyword_meanings[i];
    }

    return TOKEN_TYPE_IDENTIFIER;
}

int lex(struct token out_toks[], char const *src, size_t src_len)
{
    unsigned line = 1, column = 1;
    size_t next_tok_ind = 0, i = 0;

    /* first pass, extract textual contents into tokens. */
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
        else if (strchr(special_chars, src[i]) != NULL)
            new_tok = lex_special(src, src_len, i, line, column);
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

    /* second pass, determine actual type of extracted tokens. */
    for (i = 0; i < next_tok_ind; ++i) {
        if (out_toks[i].type == TOKEN_TYPE_IDENTIFIER)
            out_toks[i].type = identifier_type(out_toks[i].conts);
    }

    return 0;
}
