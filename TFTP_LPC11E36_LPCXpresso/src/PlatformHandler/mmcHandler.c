
#include <stdio.h>
#include "mmcHandler.h"
#include "common.h"
#include "dataflashHandler.h"

//FATFS ff;
FATFS Fatfs[1];

FRESULT getMountedMemorySize(uint8_t mount_ret, uint32_t * totalSize, uint32_t * availableSize)
{
	FATFS *fs;
	DWORD fre_clust, fre_sect, tot_sect;
	FRESULT res;

	/* Get volume information and free clusters of drive 1 */
	if(mount_ret == SPI_FLASHM)
		res = f_getfree("0:", &fre_clust, &fs);
	//else if((mount_ret >= CARD_MMC) && (mount_ret <= CARD_SDHC))
	//	res = f_getfree("1:", &fre_clust, &fs);

	if (!res)
	{
		/* Get total sectors and free sectors */
		tot_sect = (fs->n_fatent - 2) * fs->csize;
		fre_sect = fre_clust * fs->csize;

		/* Print the free space (assuming 512 bytes/sector) */
		//printf(" - Available memory size : %8ld / %8ld kB\r\n", fre_sect / 2, tot_sect / 2);

		*totalSize = tot_sect / 2;
		*availableSize = fre_sect / 2;
	}

	return res;
}

uint8_t flash_mount()
{
	FRESULT res;

	DataFlash_Init();

	//disk_initialize(1);

	res = f_mount(&Fatfs[0],"0:",0);
#if defined(_FS_DEBUG_)
    printf("\r\nf_mount:%d\r\n", res);
#endif

#if defined(F_SPI_FLASH)
    if(check_spiflash_flag() == 1)
        g_mkfs_done = 1;
    else
        g_mkfs_done = 0;

	res = f_mkfs("0:",0,512);

#if defined(_FS_DEBUG_)
    printf("f_mkfs:%d %d\r\n", res, g_mkfs_done);
#endif
    if(check_spiflash_flag() == 1)
    	save_spiflash_flag();
    g_mkfs_done = 1;
#endif

	return SPI_FLASHM;
}
