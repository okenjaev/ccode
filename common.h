#ifndef COMMON_H
#define COMMON_H

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>

#define BAZAAR_VERSION "0.0.1"
#define CTRL_KEY(k) ((k) & 0x1f)

#endif /* COMMON_H */
