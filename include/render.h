#ifndef RENDER_H
#define RENDER_H

#include "common.h"

struct str_buf;
struct buffer;

void
render_set_cursor_position(const struct buffer* buffer, struct str_buf* renderb);

void
render_draw_status_bar(const struct buffer* buffer, struct str_buf* renderb);

void
render_draw_status_message(struct buffer* buffer, struct str_buf* renderb);

void
render_draw_rows(const struct buffer* buffer, struct str_buf* renderb);

int
render_flush(const struct str_buf str);

#endif /* RENDER_H */
