#include "lex.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include <unistd.h>

struct keyword_lut_entry {
    char const *kw;
    enum token_type type;
};

struct special_char_lut_entry {
    char ch;
    enum token_type type;
};

static struct keyword_lut_entry const keywords[] = {
    {"function", TOKEN_TYPE_KEYWORD_FUNCTION},
    {"returns", TOKEN_TYPE_KEYWORD_RETURNS},
    {"entry_point", TOKEN_TYPE_KEYWORD_ENTRY_POINT},
    {"var", TOKEN_TYPE_KEYWORD_VAR},
    {"raw", TOKEN_TYPE_KEYWORD_RAW},
    {"int8", TOKEN_TYPE_KEYWORD_INT8},
    {"int16", TOKEN_TYPE_KEYWORD_INT16},
    {"int32", TOKEN_TYPE_KEYWORD_INT32},
    {"int64", TOKEN_TYPE_KEYWORD_INT64},
    {"uint8", TOKEN_TYPE_KEYWORD_UINT8},
    {"uint16", TOKEN_TYPE_KEYWORD_UINT16},
    {"uint32", TOKEN_TYPE_KEYWORD_UINT32},
    {"uint64", TOKEN_TYPE_KEYWORD_UINT64},
    {"bool", TOKEN_TYPE_KEYWORD_BOOL},
    {"true", TOKEN_TYPE_KEYWORD_TRUE},
    {"false", TOKEN_TYPE_KEYWORD_FALSE}
};

static struct special_char_lut_entry const special_chars[] = {
    {'{', TOKEN_TYPE_BRACE_LEFT},
    {'}', TOKEN_TYPE_BRACE_RIGHT},
    {'(', TOKEN_TYPE_PAREN_LEFT},
    {')', TOKEN_TYPE_PAREN_RIGHT},
    {'<', TOKEN_TYPE_ANGLE_LEFT},
    {'>', TOKEN_TYPE_ANGLE_RIGHT},
    {',', TOKEN_TYPE_COMMA},
    {'!', TOKEN_TYPE_EXCLAMATION},
    {'*', TOKEN_TYPE_ASTERISK},
    {'/', TOKEN_TYPE_SLASH},
    {'%', TOKEN_TYPE_PERCENTAGE},
    {'+', TOKEN_TYPE_PLUS},
    {'-', TOKEN_TYPE_MINUS},
    {'=', TOKEN_TYPE_EQUALS},
    {'&', TOKEN_TYPE_AMPERSAND},
    {'|', TOKEN_TYPE_PIPE},
    {'^', TOKEN_TYPE_CARET},
    {';', TOKEN_TYPE_SEMICOLON}
};

static ssize_t is_special(char ch)
{
    ssize_t i;

    for (i = 0; i < sizeof(special_chars) / sizeof(special_chars[0]); ++i) {
        if (ch == special_chars[i].ch)
            return i;
    }

    return -1;
}

void destroy_token(struct token *tok)
{
    free(tok->conts);
}

void print_token(struct token const *tok)
{
    printf("token: type   = %d\n", tok->type);
    printf("       conts  = %s\n", tok->conts);
    printf("       line   = %d\n", tok->line);
    printf("       column = %d\n", tok->column);
    printf("       length = %d\n", tok->length);
}

static void get_position(char const *src, size_t pos, unsigned *out_line,
                         unsigned *out_col)
{
    size_t i;

    *out_line = *out_col = 1;
    for (i = 0; i < pos; ++i) {
        ++*out_col;
        
        if (src[i] == '\n') {
            *out_col = 1;
            ++*out_line;
        }
    }
}

static struct token build_token(char const *src, size_t start, size_t end,
                                enum token_type type)
{
    struct token tok;
    unsigned line, col;

    get_position(src, start, &line, &col);

    tok.type = type;
    tok.line = line;
    tok.column = col;
    tok.length = end - start;
    tok.conts = malloc(tok.length + 1);
    memcpy(tok.conts, src + start, tok.length);
    tok.conts[tok.length] = '\0';

    return tok;
}

static struct token lex_identifier(char const *src, size_t src_len,
                                   size_t start)
{
    size_t end = start + 1;
    
    for (; end < src_len && (isalnum(src[end]) || src[end] == '_'); ++end);
    return build_token(src, start, end, TOKEN_TYPE_IDENTIFIER);
}

static struct token lex_string(char const *src, size_t src_len, size_t start)
{
    size_t end = start + 1;

    for (; end < src_len && src[end] != '"'; ++end);
    return build_token(src, start, end + 1, TOKEN_TYPE_STRING);
}

static struct token lex_number(char const *src, size_t src_len, size_t start)
{
    size_t end = start + 1;
    
    for (; end < src_len && (isdigit(src[end]) || src[end] == '.'); ++end);
    return build_token(src, start, end, TOKEN_TYPE_NUMBER);
}

static struct token lex_special(char const *src, size_t src_len, size_t start)
{
    size_t end = start + 1;
    enum token_type type = special_chars[is_special(src[start])].type;
    
    return build_token(src, start, end, type);
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
        if (strcmp(conts, keywords[i].kw) == 0)
            return keywords[i].type;
    }

    return TOKEN_TYPE_IDENTIFIER;
}

void lex(token_list *tl, char const *src, size_t src_len)
{
    size_t i = 0;

    /* first pass, extract textual contents into tokens. */
    while (i < src_len) {
        struct token new_tok;

        if (src[i] == '"')
            new_tok = lex_string(src, src_len, i);
        else if (isalpha(src[i]))
            new_tok = lex_identifier(src, src_len, i);
        else if (isdigit(src[i]))
            new_tok = lex_number(src, src_len, i);
        else if (is_special(src[i]) != -1)
            new_tok = lex_special(src, src_len, i);
        else if (isspace(src[i])) {
            ++i;
            continue;
        } else {
            unsigned line, col;
            
            get_position(src, i, &line, &col);
            printf("unrecognized token at l=%d,c=%d!\n", line, col);
            exit(-1);
        }

        i += new_tok.length;
        token_list_add_token(tl, &new_tok);
    }

    /* second pass, determine actual type of extracted tokens. */
    for (i = 0; i < tl->size; ++i) {
        struct token *tok = token_list_get_mut(tl, i);
        
        if (tok->type == TOKEN_TYPE_IDENTIFIER)
            tok->type = identifier_type(tok->conts);
    }
}

token_list token_list_create(void)
{
    return dynarr_create(sizeof(struct token));
}

void token_list_destroy(token_list *tl)
{
    size_t i;

    for (i = 0; i < tl->size; ++i)
        destroy_token(token_list_get_mut(tl, i));

    dynarr_destroy(tl);
}

void token_list_add_token(token_list *tl, struct token const *tok)
{
    dynarr_add_item(tl, tok);
}

struct token const *token_list_get(token_list const *tl, size_t ind)
{
    return dynarr_get(tl, ind);
}

struct token *token_list_get_mut(token_list *tl, size_t ind)
{
    return dynarr_get_mut(tl, ind);
}
