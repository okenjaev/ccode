#ifndef COMMON_H
#define COMMON_H

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define FORME_VERSION "0.0.1"
#define CTRL_KEY(k) ((k) & 0x1f)

#define NUMBER_OF_SPACES_FOR_TAB 8

#define ESQ_CONTROL "\1xb["

#endif /* COMMON_H */
