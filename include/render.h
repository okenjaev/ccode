#ifndef RENDER_H
#define RENDER_H

#include "common.h"

struct str_buf;
struct buffer;

void render_draw(const struct buffer*);
    
int
render_flush(const struct str_buf str);

#endif /* RENDER_H */
