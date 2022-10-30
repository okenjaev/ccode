#ifndef RENDERB_H
#define RENDERB_H

#include "common.h"

struct renderb
{
    char *data;
    int len;
};

#define RENDERB_INIT {NULL, 0}

void
renderb_free(struct renderb *renderb);

void
renderb_append(struct renderb *renderb, const char* s, int len);

#endif /* RENDERB_H */
