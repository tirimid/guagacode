#ifndef PARSE_H__
#define PARSE_H__

#include <stddef.h>

#include "lex.h"
#include "dynarr.h"

struct node {
    token_list tokens;
    struct dynarr children;
};

struct node node_create(void);
void node_destroy(struct node *node);
struct token const *node_get_token(struct node const *node, size_t ind);
struct node const *node_get_child(struct node const *node, size_t ind);
void node_add_token(struct node *node, struct token const *tok);
void node_add_child(struct node *node, struct node const *child);

#endif
