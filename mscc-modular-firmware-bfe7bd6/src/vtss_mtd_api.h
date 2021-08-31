// Copyright (c) 2016 Microsemi Corporation "Microsemi".

#ifndef __VTSS_MTD_API_H__
#define __VTSS_MTD_API_H__

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "vtss_compat.h"

#ifdef __cplusplus
extern "C" {
#endif
#if 0
}  /* Fixing extra indent */
#endif

#include "stdlib.h"
#include <mtd/mtd-user.h>
typedef struct {
    int fd;  /* Fileno of /dev/<name> */
    int devno;
    char dev[64];  /* mtd device name */
    struct mtd_info_user info;
} vtss_mtd_t;

#define FLAGS (O_RDWR | O_SYNC)

/****************************************************************************
 * Public functions
 ****************************************************************************/
/*!
 * \brief Open a mtd device.
 *
 * \param *mtd [IN] pointer to a mtd device.
 *
 * \param *name [IN] name of the mtd device.
 *
 * \return VTSS_OK if the operation succeeded.
 *         VTSS_RC_ERROR if the operation failed.
 */
vtss_rc vtss_mtd_open(vtss_mtd_t *mtd, const char* name);

/*!
 * \brief Erase a mtd device.
 *
 * \param *mtd [IN] pointer to a mtd device.
 *
 * \param length [IN] size of the mtd device.
 *
 * \return VTSS_OK if the operation succeeded.
 *         VTSS_RC_ERROR if the operation failed.
 */
vtss_rc vtss_mtd_erase(const vtss_mtd_t *mtd, size_t length);

/*!
 * \brief Program a mtd device.
 *
 * \param *mtd [IN] pointer to a mtd device.
 *
 * \param *buffer [IN] program mtd device from the buffer pointed by *buffer.
 *
 * \param length [IN] program length bytes.
 *
 * \return VTSS_OK if the operation succeeded.
 *         VTSS_RC_ERROR if the operation failed.
 */
vtss_rc vtss_mtd_program(const vtss_mtd_t *mtd, const u8 *buffer, size_t length);

/*!
 * \brief Close a mtd device.
 *
 * \param *mtd [IN] pointer to a mtd device.
 *
 * \return VTSS_OK if the operation succeeded.
 *         VTSS_RC_ERROR if the operation failed.
 */
vtss_rc vtss_mtd_close(vtss_mtd_t *mtd);


#if 0
{  /* Fixing extra indent */
#endif

#ifdef __cplusplus
}
#endif
#endif  /* __VTSS_MTD_API_H__ */
