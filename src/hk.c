#include "hk.h"

struct fhk
fhk_init(const fchar* text, PyObject* call_back)
{
    struct fhk res;
    res.text = malloc(sizeof(text));
    memcpy(res.text, text, sizeof(text));
    res.call_back = call_back;
    return res;
}
