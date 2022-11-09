#ifndef STATMESSAGE_H
#define STATMESSAGE_H

#include "common.h"

struct sm
{
    time_t time;
    char message[80];
};

void
sm_set_message(const char* fmt, ...);

#endif /* STATMESSAGE_H */
