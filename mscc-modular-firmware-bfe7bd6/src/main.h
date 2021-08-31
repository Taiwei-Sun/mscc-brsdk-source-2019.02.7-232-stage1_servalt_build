// Copyright (c) 2016 Microsemi Corporation "Microsemi".

#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdlib.h>
#include <zlib.h>
#include <string.h>
#include <inttypes.h>

#define LOG_ERROR 2
#define LOG_WARN  4
#define LOG_INFO  6
#define LOG_DEBUG 7
#define LOG_NOISE 8

extern unsigned int g_log_level;
extern char g_switch_profile[64];

int warn(const char *fmt, ...);
int info(const char *fmt, ...);
int debug(const char *fmt, ...);
int noise(const char *fmt, ...);

#define DIR_STAGE2        "/tmp/stage2"
#define DIR_ROOT_ELEM     DIR_STAGE2 "/rootfs-elements"
#define DIR_ROOT_ELEM_IX  DIR_ROOT_ELEM "/%d"
#define DIR_NEW_ROOT      DIR_STAGE2 "/new-root"

typedef struct Buf {
    size_t size;
    char *data;
} Buf;

int rootfs_extract(const char *d, size_t s, const char *prefix);
int process_stage2_fd(int fd, int fd_loop, size_t off, size_t len);

#endif  // __MAIN_H__
