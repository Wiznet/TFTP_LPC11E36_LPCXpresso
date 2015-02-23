/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* by grqd_xp                                                            */
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/
#include <string.h>

#include "diskio.h"
#include "common.h"
#include "dataflashHandler.h"
#include "ffconf.h"

/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */
/* Note that Tiny-FatFs supports only single drive and always            */
/* accesses drive number 0.                                              */

#define SECTOR_SIZE 512U

#if 1
int rtc_year = 2014;
int rtc_month = 12;
int rtc_day = 31;
int rtc_hour = 10;
int rtc_min = 10;
int rtc_sec = 30;

//u32 buff2[512/4];
/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	uint8_t state;

#if 0
    if(pdrv)
    {
        return STA_NOINIT;  // only supports the operation of the disk 0
    }
#endif

   	return 0;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{	
#if 0
    if(pdrv)
    {
        return STA_NOINIT;  // only supports disk-0 operation
    }
#endif

	// Check whether the inserted SD card
	//  if(!SD_DET())
	//  {
	//     return STA_NODISK;
	//  }
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	uint8_t res=0;
    if (!count)
    {    
        return RES_PARERR;  // only supports single disk operation, count is not equal to 0, otherwise parameter error
    }
   // if(!SD_DET())
   // {
   //     return RES_NOTRDY;  // does not detect SD card, NOT READY error reported
   // }

	//printf("-----> disk_read drv:%d sector:%d count:%d buf:%s\r\n", pdrv, sector, count, buff);

    if(pdrv == 0)
    {
		//memset(buff, 0xff, _MAX_SS);
		read_from_flashbuf(sector*_MAX_SS, (char*)buff, count*_MAX_SS);

		return 0;
    }
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	uint8_t res;

    if (!count)
    {    
        return RES_PARERR;  // only supports single disk operation, count is not equal to 0, otherwise parameter error
    }
	/*
    if(!SD_DET())
    {
        return RES_NOTRDY;  // does not detect SD card, NOT READY error reported
    }  */

	//printf("-----> disk_write drv:%d sector:%d count:%d buf:%s\r\n", pdrv, sector, count, buff);

    if(pdrv == 0)
    {
		if(g_mkfs_done == 1)
		{
			write_to_flashbuf(sector*_MAX_SS, (char*)buff, count*_MAX_SS);
		}

		return 0;
    }
}
#endif /* _READONLY */


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{

    DRESULT res;

#if 0
    if (pdrv)
    {    
        return RES_PARERR;  // only supports single disk operation, or return parameter error
    }
#endif

    if(pdrv == 0)
    {
		switch(cmd)
		{
		case CTRL_SYNC:
#if defined(F_SPI_FLASH)
			Flash_WaitReady();
#endif
			res = RES_OK;
			break;

		case GET_SECTOR_SIZE:
			*(WORD*)buff = 512;
			res = RES_OK;
			break;

		case GET_BLOCK_SIZE:
			*(WORD*)buff = 1;
			res = RES_OK;
			break;

		case GET_SECTOR_COUNT:
			*(DWORD*)buff = 1024;//2048;
			res = RES_OK;
			break;

		default:
			res = RES_PARERR;
			break;
		}
    }

    return res;
}
#endif

DWORD get_fattime(void)
{
	DWORD tmr = 0;

	//__disable_irq();
	tmr = (((DWORD)rtc_year - 1980) << 25)
		 | ((DWORD)rtc_month << 21)
		 | ((DWORD)rtc_day << 16)
		 | ((WORD)rtc_hour << 11)
		 | ((WORD)rtc_min << 5)
		 | ((WORD)rtc_sec << 1);
	//__enable_irq();

	return tmr;
}

#endif





















