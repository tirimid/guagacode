#ifndef PARSE_H__
#define PARSE_H__

#include <stddef.h>

#include "lex.h"
#include "dynarr.h"

typedef struct dynarr node_list;

struct node {
    token_list tokens;
    node_list children;
};

struct node node_create(void);
void node_destroy(struct node *node);
struct token const *node_get_token(struct node const *node, size_t ind);
struct node const *node_get_child(struct node const *node, size_t ind);
void node_add_token(struct node *node, struct token const *tok);
void node_add_child(struct node *node, struct node const *child);

node_list node_list_create(void);
void node_list_destroy(node_list *nl);
void node_list_add_node(node_list *nl, struct node const *node);
struct node const *node_list_get(node_list const *nl, size_t ind);
struct node *node_list_get_mut(node_list *nl, size_t ind);

#endif
