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
#include <fcntl.h>

#define FORME_VERSION "0.0.5"
#define FORME_CTRL_KEY(k) ((k) & 0x1f)

#define FORME_QUIT_TIMES 3
#define FORME_NUMBER_OF_SPACES_FOR_TAB 4

// temp

struct cur_pos {
    int x;
    int y;
    int r;
    int rowoff;
    int coloff;
};


#endif /* COMMON_H */
