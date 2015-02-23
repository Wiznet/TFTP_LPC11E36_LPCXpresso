/*
 * File:        common.h
 * Purpose:     File included by non-standard NXP drivers.
 *              Do not use in your projects, use CMSIS files!
 *
 * Notes:
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#define F_SPI_FLASH
#define _FS_DEBUG_

#define DATA_BUF_SIZE   1024

extern int g_mkfs_done;
extern int g_sdcard_done;

#if defined(F_SPI_FLASH)
extern int check_spiflash_flag(void);
extern void save_spiflash_flag(void);
extern void release_factory_flag(void);
#endif

#endif /* _COMMON_H_ */
