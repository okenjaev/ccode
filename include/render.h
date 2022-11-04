#ifndef RENDER_H
#define RENDER_H

#include "common.h"

struct str;
struct buffer;

void
render_set_cursor_position(const struct buffer* buffer, struct str* renderb);

void
render_draw_status_bar(const struct buffer* buffer, struct str* renderb);

void
render_draw_status_message(const struct buffer* buffer, struct str* renderb);

void
render_draw_rows(const struct buffer* buffer, struct str* renderb);

int
render_flush(const struct str str);

#endif /* RENDER_H */
