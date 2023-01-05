#include "dynarr.h"

#include <stdlib.h>
#include <string.h>

struct dynarr dynarr_create(size_t stride)
{
    struct dynarr da;

    da.stride = stride;
    da.size = 0;
    da.cap = 1;
    da.data = malloc(stride);

    return da;
}

void dynarr_destroy(struct dynarr *da)
{
    free(da->data);
}

void dynarr_add_item(struct dynarr *da, void const *item)
{
    /* copy the new element into the data of the dynamic array. */
    memcpy(dynarr_get_mut(da, da->size), item, da->stride);
    ++da->size;

    /* if the dynamic array is full after the addition, give it more memory. */
    if (da->size == da->cap) {
        da->cap *= 2;
        da->data = realloc(da->data, da->cap * da->stride);
    }
}

void const *dynarr_get(struct dynarr const *da, size_t ind)
{
    return (void const *)((char const *)da->data + da->stride * ind);
}

void *dynarr_get_mut(struct dynarr *da, size_t ind)
{
    return (void *)((char *)da->data + da->stride * ind);
}
