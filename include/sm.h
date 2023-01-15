#ifndef SM_H
#define SM_H

#include "common.h"

struct sm
{
    time_t time;
    fchar message[80];
};

void
sm_set_message(const fchar* fmt, ...);

#endif /* SM_H */
