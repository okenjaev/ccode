#include "statmessage.h"

struct sm sm;

void
sm_set_message(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(sm.message, sizeof(sm.message), fmt, ap);
    va_end(ap);
    sm.time = time(NULL);
}
