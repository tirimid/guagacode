#include "parse.h"

struct node node_create(void)
{
    struct node node;

    node.tokens = token_list_create();
    node.children = node_list_create();
    return node;
}

void node_destroy(struct node *node)
{
    size_t i;

    for (i = 0; i < node->children.size; ++i) {
        node_destroy(node_list_get_mut(&node->children, i));
        node->children.cap = 0;
    }

    token_list_destroy(&node->tokens);
    node_list_destroy(&node->children);
}

struct token const *node_get_token(struct node const *node, size_t ind)
{
    return token_list_get(&node->tokens, ind);
}

struct node const *node_get_child(struct node const *node, size_t ind)
{
    return node_list_get(&node->children, ind);
}

void node_add_token(struct node *node, struct token const *tok)
{
    token_list_add_token(&node->tokens, tok);
}

void node_add_child(struct node *node, struct node const *child)
{
    node_list_add_node(&node->children, child);
}

node_list node_list_create(void)
{
    return dynarr_create(sizeof(struct node));
}

void node_list_destroy(node_list *nl)
{
    if (nl->cap != 0) {
        size_t i;
        
        for (i = 0; i < nl->size; ++i)
            node_destroy(node_list_get_mut(nl, i));
    }

    dynarr_destroy(nl);
}

void node_list_add_node(node_list *nl, struct node const *node)
{
    dynarr_add_item(nl, node);
}

struct node const *node_list_get(node_list const *nl, size_t ind)
{
    return dynarr_get(nl, ind);
}

struct node *node_list_get_mut(node_list *nl, size_t ind)
{
    return dynarr_get_mut(nl, ind);
}
