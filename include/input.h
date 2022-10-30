#ifndef INPUT_H
#define INPUT_H

#include "common.h"

enum editor_keys
{
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    PAGE_UP,
    PAGE_DOWN,
    HOME_KEY,
    END_KEY,
    DEL_KEY
};


int
editor_read_key();

#endif /* INPUT_H */
