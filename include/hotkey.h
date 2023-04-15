#ifndef HOTKEY_H
#define HOTKEY_H

#include "common.h"

struct hotkey
{
    fchar* text;
    void* call_back;
};

struct hotkey
hotkey_init(const fchar* text, void* call_back);

#endif /* HOTKEY_H */
