#include <stdint.h>
#include "ff.h"
#include "diskio.h"

#define GO_IDLE_STATE			0
#define SEND_OP_COND			1
#define SEND_IF_COND			8
#define SET_BLOCKLEN			16
#define READ_BLOCK				17
#define WRITE_BLOCK				24
#define APP_CMD					55
#define READ_OCR				58
#define SD_SEND_OP_COND			(0x40|41)

typedef enum card_type_id {
	NO_CARD,
	CARD_MMC,
	CARD_SD,
	CARD_SD2,
	CARD_SDHC,
	SPI_FLASHM
} card_type_id_t;

uint8_t flash_mount();
FRESULT getMountedMemorySize(uint8_t mount_ret, uint32_t * totalSize, uint32_t * availableSize);
