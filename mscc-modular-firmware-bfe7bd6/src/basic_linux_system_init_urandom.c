// Copyright (c) 2016 Microsemi Corporation "Microsemi".

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "mfi.h"

typedef enum
{
    false,
    true
} bool;

void basic_linux_system_init_urandom(const char *SEED) {
    bool seed_ok = false;
    const size_t buf_size = 512;
    static const char *DEV = "/dev/urandom";
    // static const char *SEED = /* VTSS_FS_FLASH_DIR */ "/random-seed";
    char buf[buf_size];
    char *p;
    size_t s;
    int fd = -1;

    // Read the seed from last time (if it exists)
    seed_ok = false;
    fd = open(SEED, O_RDONLY);
    if (fd != -1) {
        p = buf;
        s = buf_size;

        while (s) {
            int res = read(fd, buf, s);
            if (res == -1) {
                warn("URANDOM: Failed to read from %s: %s", SEED,
                     strerror(errno));
                break;
            } else {
                p += res;
                s -= res;
            }
        }

        if (s == 0) {
            seed_ok = true;
        }

        close(fd);
        fd = -1;
    }

    // Write the seed into the urandom device
    if (seed_ok) {
        fd = open(DEV, O_WRONLY);
        if (fd != -1) {
            p = buf;
            s = buf_size;

            while (s) {
                int res = write(fd, buf, s);
                if (res == -1) {
                    warn("URANDOM: Failed to write to %s: %s", DEV,
                         strerror(errno));
                    break;
                } else {
                    p += res;
                    s -= res;
                }
            }

            close(fd);
            fd = -1;
        }
    }

    // Read a new seed from urandom
    seed_ok = false;
    fd = open(DEV, O_RDONLY);
    if (fd != -1) {
        p = buf;
        s = buf_size;

        while (s) {
            int res = read(fd, buf, s);
            if (res == -1) {
                warn("URANDOM: Failed to read from %s: %s", DEV, strerror(errno));
                break;
            } else {
                p += res;
                s -= res;
            }
        }

        if (s == 0) {
            seed_ok = true;
        }

        close(fd);
        fd = -1;
    } else {
        warn("URANDOM: Failed to open %s: %s\n", DEV, strerror(errno));
    }

    // Write the seed such that it will be used on the next boot
    if (seed_ok) {
        fd = open(SEED, O_WRONLY | O_CREAT | O_TRUNC);
        if (fd != -1) {
            p = buf;
            s = buf_size;

            while (s) {
                int res = write(fd, buf, s);
                if (res == -1) {
                    warn("URANDOM: Failed to write to %s: %s", SEED,
                         strerror(errno));
                    break;
                } else {
                    p += res;
                    s -= res;
                }
            }

            close(fd);
            fd = -1;

        } else {
            warn("URANDOM: Failed to open %s: %s\n", SEED, strerror(errno));
        }
    }
}

