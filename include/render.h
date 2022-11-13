#ifndef RENDER_H
#define RENDER_H

#include "common.h"

struct str_buf;

void render_draw(void);

int
render_flush(const struct str_buf str);

#endif /* RENDER_H */
