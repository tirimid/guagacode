#include "parse.h"

struct node node_create(void)
{
    struct node node;

    node.tokens = token_list_create();
    node.children = dynarr_create(sizeof(struct node));
    return node;
}

void node_destroy(struct node *node)
{
    size_t i;

    for (i = 0; i < node->children.size; ++i)
        node_destroy(dynarr_get_mut(&node->children, i));

    token_list_destroy(&node->tokens);
    dynarr_destroy(&node->children);
}

struct token const *node_get_token(struct node const *node, size_t ind)
{
    return token_list_get(&node->tokens, ind);
}

struct node const *node_get_child(struct node const *node, size_t ind)
{
    return dynarr_get(&node->children, ind);
}

void node_add_token(struct node *node, struct token const *tok)
{
    token_list_add_token(&node->tokens, tok);
}

void node_add_child(struct node *node, struct node const *child)
{
    dynarr_add_item(&node->children, child);
}
