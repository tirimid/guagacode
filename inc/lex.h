#ifndef LEX_H__
#define LEX_H__

#include <stddef.h>

#include "dynarr.h"

typedef struct dynarr token_list;

enum token_type {
    /* language keywords. */
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
    TOKEN_TYPE_KEYWORD_FALSE,

    /* special characters. */
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
void lex(token_list *tl, char const *src, size_t src_len);

token_list token_list_create(void);
void token_list_destroy(token_list *tl);
void token_list_add_token(token_list *tl, struct token const *tok);
struct token const *token_list_get(token_list const *tl, size_t ind);
struct token *token_list_get_mut(token_list *tl, size_t ind);

#endif
