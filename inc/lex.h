#ifndef LEX_H__
#define LEX_H__

enum token_type {
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_NUMBER
};

/* free all allocated memory by calling `destroy_token()`. */
struct token {
    enum token_type type;
    char *conts;
    unsigned long line, pos_on_line, length;
};

void destroy_token(struct token *tok);
void print_token(struct token const *tok);
int lex(struct token out_toks[], char const *src, unsigned long src_len);

#endif
