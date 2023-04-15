#include "hotkey.h"

struct hotkey
hotkey_init(const fchar* text, void* call_back)
{
    struct hotkey res;
    res.text = malloc(sizeof(text));
    memcpy(res.text, text, sizeof(text));
    res.call_back = call_back;
    return res;
}
