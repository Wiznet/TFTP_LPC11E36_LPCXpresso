/*
===============================================================================
 Name        : W5500EVB.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include "spiHandler.h"
#include "w5500_init.h"
#include "common.h"
#include "loopback.h"
#include "mmcHandler.h"
#include "dataflashHandler.h"
#include "ftpd.h"
#include "wizchip_conf.h"
#include "ffconf.h"
#include "eepromHandler.h"
#include "tftp.h"

#include <cr_section_macros.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

#if defined(F_SPI_FLASH)
typedef struct __Cfg_Info {
	uint8_t spiflash_flag[2];
} __attribute__((packed)) Cfg_Info;
#endif

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#define TICKRATE_HZ1 (1000)		/* 1000 ticks per second, for SysTick */
#define TICKRATE_HZ2 (1)		/* 1 ticks per second, for Timer0 */
volatile uint32_t msTicks; 		/* counts 1ms timeTicks */

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
///////////////////////////////////////
// Debugging Message Printout enable //
///////////////////////////////////////
#define _MAIN_DEBUG_

///////////////////////////
// Demo Firmware Version //
///////////////////////////
#define VER_H		1
#define VER_L		00

//////////////////////////////////////////////////
// Socket & Port number definition for Examples //
//////////////////////////////////////////////////
#define SOCK_TCPS       0
#define SOCK_UDPS       1
#define PORT_TCPS		5000
#define PORT_UDPS       3000

////////////////////////////////////////////////
// Shared Buffer Definition for LOOPBACK TEST //
////////////////////////////////////////////////

uint8_t gDATABUF[DATA_BUF_SIZE];
#if defined(F_APP_FTP)
uint8_t gFTPBUF[_MAX_SS];
#endif
#if defined(F_APP_TFTP)
uint8_t g_socket_rcv_buf[MAX_MTU_SIZE];
uint8_t g_op_mode = NORMAL_MODE;
#endif

///////////////////////////
// Network Configuration //
///////////////////////////
wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef},
                            .ip = {192, 168, 0, 91},
                            .sn = {255, 255, 255, 0},
                            .gw = {192, 168, 0, 1},
                            .dns = {8, 8, 8, 8},
                            .dhcp = NETINFO_STATIC };

// For TCP client loopback examples; destination network info
uint8_t destip[4] = {192, 168, 0, 248};
uint16_t destport = 5000;

int g_mkfs_done = 0;
int g_sdcard_done = 0;

#if defined(F_APP_TFTP)
uint8_t filename[50];
#endif

////////////////////
// Button Control //
////////////////////
#define BUTTONS_PRESSED_TICKS		10		// ms
bool button1_enable = false;
bool button1_pressed_flag = false;
bool button2_enable = false;
bool button2_pressed_flag = false;

static void display_SDcard_Info(uint8_t mount_ret);
static uint8_t Check_Buttons_Pressed(void);
void SysTick_Handler(void);

int main(void) {
	uint8_t ret = 0;
    //int32_t loopback_ret;
	uint8_t bt = 0;
#if defined(F_APP_TFTP)
	uint32_t tftp_server;
#endif

#if defined (__USE_LPCOPEN)
#if !defined(NO_BOARD_LIB)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
    //Board_LED_Set(1, true);
    //Board_LED_Set(2, true);
#endif
#endif

	SPI_Init();
	W5500_Init();
	Net_Conf(gWIZNETINFO);

#ifdef _MAIN_DEBUG_
	uint8_t tmpstr[6] = {0,};

	ctlwizchip(CW_GET_ID,(void*)tmpstr);

    printf("\r\n=======================================\r\n");
	printf(" WIZnet %s EVB Demos v%d.%.2d\r\n", tmpstr, VER_H, VER_L);
	printf("=======================================\r\n");
	printf(">> W5500 based TFTP Example\r\n");
	printf("=======================================\r\n");

	Display_Net_Conf(); // Print out the network information to serial terminal
#endif

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ1);

	/* Initialize buttons on the W5500 EVB board */
	Board_Buttons_Init();

	g_sdcard_done = 0;

#if defined(F_APP_FTP)
	ftpd_init(gWIZNETINFO.ip);
#endif

#if defined(F_APP_TFTP)
	TFTP_init(SOCK_TFTP, g_socket_rcv_buf);
#endif

	ret = flash_mount();
	if(ret > 0)
	{
		display_SDcard_Info(ret);
	}

	while(1) {

		bt = Check_Buttons_Pressed();

		/* Button: SW1 */
		if(bt == BUTTONS_BUTTON1)
		{
			printf("\r\n########## SW1 was pressed.\r\n");
#if defined(F_SPI_FLASH)
			printf("########## Data flash flag was cleared.\r\n");
			printf("########## Please reset a target.\r\n");
			release_factory_flag();
#endif
		}
		else if(bt == BUTTONS_BUTTON2)
		{
			printf("\r\n########## SW2 was pressed.\r\n");
#if defined(F_APP_TFTP)
			strncpy(filename, "test.txt", 8);

			tftp_server = (destip[0] << 24) | (destip[1] << 16) | (destip[2] << 8) | (destip[3]);

			TFTP_read_request(tftp_server, filename);

			while(1) {
				ret = TFTP_run();
				if(ret != TFTP_PROGRESS)
					break;
			}
#endif
		}

#if defined(F_APP_FTP)
		ftpd_run(gFTPBUF);
#endif

		/* Loopback Test: TCP Server and UDP */
		// Test for Ethernet data transfer validation
		{
			//loopback_tcps(SOCK_TCPS, gDATABUF, PORT_TCPS);
			//loopback_udps(SOCK_UDPS, gDATABUF, PORT_UDPS);
			//loopback_ret = loopback_tcpc(SOCK_TCPS, gDATABUF, destip, destport);

			//if(loopback_ret < 0) printf("loopback ret: %ld\r\n", loopback_ret); // TCP Socket Error code
		}
	}

    return 0 ;
}

static void display_SDcard_Info(uint8_t mount_ret)
{
	uint32_t totalSize = 0, availableSize = 0;

	printf("\r\n - Storage mount succeed\r\n");
	printf(" - Type : ");

	switch(mount_ret)
	{
		case CARD_MMC: printf("MMC\r\n"); 	break;
		case CARD_SD: printf("SD\r\n"); 	break;
		case CARD_SD2: printf("SD2\r\n"); 	break;
		case CARD_SDHC: printf("SDHC\r\n"); break;
		case SPI_FLASHM: printf("sFlash\r\n"); break;
		default: printf("\r\n"); 	break;
	}

	if(_MAX_SS == 512)
	{
		getMountedMemorySize(mount_ret, &totalSize, &availableSize);
		printf(" - Available Memory Size : %ld kB / %ld kB ( %ld kB is used )\r\n", availableSize, totalSize, (totalSize - availableSize));
	}
	printf("\r\n");
}

static uint8_t Check_Buttons_Pressed(void)
{
	static uint8_t buttons_status;
	static uint8_t ret;

	buttons_status = Buttons_GetStatus();

	if((buttons_status & BUTTONS_BUTTON1) == BUTTONS_BUTTON1) button1_enable = true; // button pressed check timer enable
	else if((buttons_status & BUTTONS_BUTTON2) == BUTTONS_BUTTON2) button2_enable = true;
	else {button1_enable = false; button2_enable = false;}

	if(button1_pressed_flag)	// button1 pressed (Specified time elapsed, enabled by sysTick_Handler function)
	{
		button1_pressed_flag = false; // pressed button clear
		ret = BUTTONS_BUTTON1; // return pressed button status
	}
	else if(button2_pressed_flag)	// button2 pressed (Specified time elapsed, enabled by sysTick_Handler function)
	{
		button2_pressed_flag = false;
		ret = BUTTONS_BUTTON2;
	}
	else
	{
		ret = 0;
	}

	return ret;
}

void SysTick_Handler(void)
{
	static uint16_t button1_pressed_check_cnt = 0;
	static bool button1_press_detected = false;
	static uint16_t button2_pressed_check_cnt = 0;
	static bool button2_press_detected = false;

	msTicks++; // increment counter

	// Button1 control
	if(button1_enable == true)
	{
		if(!button1_press_detected)
		{
			button1_pressed_check_cnt++;
			if(button1_pressed_check_cnt >= BUTTONS_PRESSED_TICKS)
			{
				button1_pressed_flag = true;
				button1_pressed_check_cnt = 0;
				button1_enable = false;

				button1_press_detected = true;
			}
		}
	}
	else
	{
		button1_pressed_check_cnt = 0;
		button1_press_detected = false;
	}

	// Button2 control
	if(button2_enable == true)
	{
		if(!button2_press_detected)
		{
			button2_pressed_check_cnt++;
			if(button2_pressed_check_cnt >= BUTTONS_PRESSED_TICKS)
			{
				button2_pressed_flag = true;
				button2_pressed_check_cnt = 0;
				button2_enable = false;

				button2_press_detected = true;
			}
		}
	}
	else
	{
		button2_pressed_check_cnt = 0;
		button2_press_detected = false;
	}
}

#if defined(F_SPI_FLASH)
int check_spiflash_flag(void)
{
	int ret = 0;
	Cfg_Info cfg_info;

	read_eeprom(0, &cfg_info, sizeof(Cfg_Info));

	if(cfg_info.spiflash_flag[0] == 0xAE && cfg_info.spiflash_flag[1] == 0xAE)
	{
		ret = 0;
	}
	else
	{
		ret = 1;
	}

	return ret;
}

void save_spiflash_flag(void)
{
	Cfg_Info cfg_info;

	cfg_info.spiflash_flag[0] = 0xAE;
	cfg_info.spiflash_flag[1] = 0xAE;

	write_eeprom(0, &cfg_info, sizeof(Cfg_Info));
}

void release_factory_flag(void)
{
	Cfg_Info cfg_info;

	cfg_info.spiflash_flag[0] = 0xFF;
	cfg_info.spiflash_flag[1] = 0xFF;

	write_eeprom(0, &cfg_info, sizeof(Cfg_Info));
}
#endif
