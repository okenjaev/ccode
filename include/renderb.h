#ifndef RENDERB_H
#define RENDERB_H

#include "common.h"

#define RENDERB_INIT {NULL, 0}

struct renderb
{
    char *data;
    int len;
};

void
renderb_free(struct renderb *renderb);

void
renderb_append(struct renderb *renderb, const char* s, int len);

int
renderb_flush(const struct renderb* renderb);

#endif /* RENDERB_H */
