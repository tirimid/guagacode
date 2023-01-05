#ifndef DYNARR_H__
#define DYNARR_H__

#include <stddef.h>

struct dynarr {
    size_t size, cap, stride;
    void *data;
};

struct dynarr dynarr_create(size_t stride);
void dynarr_destroy(struct dynarr *da);
void dynarr_add_item(struct dynarr *da, void const *item);
void const *dynarr_get(struct dynarr const *da, size_t ind);
void *dynarr_get_mut(struct dynarr *da, size_t ind);

#endif
