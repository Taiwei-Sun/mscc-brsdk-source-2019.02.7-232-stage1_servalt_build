// Copyright (c) 2016 Microsemi Corporation "Microsemi".

#ifndef __MFI_SRC_MFI_HXX__
#define __MFI_SRC_MFI_HXX__

#include "main.h"

int info(const char *format, ...) __attribute__((format(printf, 1, 2)));
int debug(const char *format, ...) __attribute__((format(printf, 1, 2)));
int warn(const char *format, ...) __attribute__((format(printf, 1, 2)));

void basic_linux_system_init_urandom(const char *SEED);

int try_run(const char *e);

#endif  // __MFI_SRC_MFI_HXX__
