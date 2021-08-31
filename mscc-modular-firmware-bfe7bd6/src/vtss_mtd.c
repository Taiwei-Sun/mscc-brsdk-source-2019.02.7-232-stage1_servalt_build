// Copyright (c) 2016 Microsemi Corporation "Microsemi".

#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "vtss_mtd_api.h"

vtss_rc vtss_mtd_open(vtss_mtd_t *mtd, const char* name)
{
    FILE *procmtd;
    char mtdentry[128] = {0};
    char mtdname[32] = {0};

    memset(mtd, 0, sizeof(*mtd));
    mtd->devno = mtd->fd = -1;

    sprintf(mtdname, "\"%s\"", name);
    if ((procmtd = fopen("/proc/mtd", "r"))) {
        while (fgets(mtdentry, sizeof(mtdentry), procmtd)) {
            if (sscanf(mtdentry, "mtd%d:", &mtd->devno) && strstr(mtdentry, mtdname)) {
                snprintf(mtd->dev, sizeof(mtd->dev), "mtd%d", mtd->devno);
                snprintf(mtdentry, sizeof(mtdentry), "/dev/%s", mtd->dev);
                if ((mtd->fd = open(mtdentry, FLAGS)) >= 0) {
                    if(ioctl(mtd->fd, MEMGETINFO, &mtd->info) == 0) {
                        break;    // Open & info
                    }
                    T_E("NO MTD info from %s", mtdentry);
                    close(mtd->fd);
                }
            }
        }
        fclose(procmtd);
    }

    return (mtd->fd >= 0) ? VTSS_OK : (vtss_rc) FIRMWARE_ERROR_CURRENT_NOT_FOUND;
}

vtss_rc vtss_mtd_erase(const vtss_mtd_t *mtd, size_t length)
{
    struct erase_info_user mtdEraseInfo;
    size_t offset;

    for (offset = 0; offset < length; offset += mtd->info.erasesize) {
        mtdEraseInfo.start = offset;
        mtdEraseInfo.length = mtd->info.erasesize;
        (void) ioctl(mtd->fd, MEMUNLOCK, &mtdEraseInfo);
        if (ioctl (mtd->fd, MEMERASE, &mtdEraseInfo) < 0) {
            T_E("%s: Erase error at 0x%08zx: %s", mtd->dev, offset, strerror(errno));
            return FIRMWARE_ERROR_FLASH_ERASE;
        }
    }

    return VTSS_OK;
}

vtss_rc vtss_mtd_program(const vtss_mtd_t *mtd, const u8 *buffer, size_t length)
{
    size_t written, hdrsize = (1 * 1024);

    if (length > hdrsize) {
        // Write payload first, *then* header
        T_D("%s: Write %zd (data) then %zd (header)", mtd->dev, length - hdrsize, hdrsize);
        if ((written = pwrite(mtd->fd, buffer + hdrsize, length - hdrsize, hdrsize)) == (length - hdrsize)) {
            T_D("%s: Data: Wrote len %zd", mtd->dev, written);
            written += pwrite(mtd->fd, buffer, hdrsize, 0);
            T_D("%s: Header: Wrote len %zd (total)", mtd->dev, written);
        }
    } else {
        T_D("%s: Write len %zd", mtd->dev, length);
        written = pwrite(mtd->fd, buffer, length, 0);
    }

    if (written != length) {
        T_E("%s: Write error len %zd: %s\n", mtd->dev, length, strerror(errno));
        return FIRMWARE_ERROR_FLASH_PROGRAM;
    }

    return VTSS_OK;
}

vtss_rc vtss_mtd_close(vtss_mtd_t *mtd)
{
    return close(mtd->fd) ? VTSS_RC_ERROR : VTSS_OK;
}
