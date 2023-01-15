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
#include <stdint.h>

#define FORME_VERSION "0.0.5"
#define FORME_CTRL_KEY(k) ((k) & 0x1f)

#define FORME_QUIT_TIMES 3
#define FORME_NUMBER_OF_SPACES_FOR_TAB 4

typedef char fchar;
typedef int8_t fint8;
typedef int16_t fint16;
typedef int32_t fint32;
typedef int64_t fint64;

typedef uint8_t fuint8;
typedef uint16_t fuint16;
typedef uint32_t fuint32;
typedef uint64_t fuint64;

typedef double fdouble;
typedef float ffloat;

#endif /* COMMON_H */
