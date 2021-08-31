// Copyright (c) 2016 Microsemi Corporation "Microsemi".

#ifndef _VTSS_COMPAT_HXX_
#define _VTSS_COMPAT_HXX_

#include "main.h"
#include "mfi.h"

#ifdef __cplusplus
extern "C" {
#endif

// Compat
typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned char vtss_rc;

#define VTSS_OK                          0
#define VTSS_RC_ERROR                    1
#define FIRMWARE_ERROR_FLASH_ERASE       101
#define FIRMWARE_ERROR_FLASH_PROGRAM     102
#define FIRMWARE_ERROR_CURRENT_NOT_FOUND 103

#define T_E(s, ...) warn(s "\n", ##__VA_ARGS__)
#define T_W(s, ...) warn(s "\n", ##__VA_ARGS__)
#define T_I(s, ...) info(s "\n", ##__VA_ARGS__)
#define T_D(s, ...) debug(s "\n", ##__VA_ARGS__)

#define VTSS_MALLOC  malloc
#define VTSS_REALLOC realloc
#define VTSS_FREE    free

#define MD5_MAC_LEN 16

#ifdef __cplusplus
}
#endif

#endif    // _VTSS_COMPAT_HXX_
