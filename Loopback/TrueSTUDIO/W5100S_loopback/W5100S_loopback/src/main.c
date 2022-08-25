#include <stdio.h>
#include <stdlib.h>
#include "HAL_Config.h"
#include "HALInit.h"
#include "wizchip_conf.h"
#include "inttypes.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "W5100SRelFunctions.h"
#include "serialCommand.h"

wiz_NetInfo gWIZNETINFO = { .mac = {0x00,0x08,0xdc,0x78,0x91,0x71},
							.ip = {192,168,11,15},
							.sn = {255, 255, 255, 0},
							.gw = {192, 168, 11, 1},
							.dns = {168, 126, 63, 1},
							.dhcp = NETINFO_STATIC};

#define ETH_MAX_BUF_SIZE	2048

unsigned char ethBuf0[ETH_MAX_BUF_SIZE];
unsigned char ethBuf1[ETH_MAX_BUF_SIZE];
unsigned char ethBuf2[ETH_MAX_BUF_SIZE];
unsigned char ethBuf3[ETH_MAX_BUF_SIZE];

uint8_t bLoopback = 1;
uint8_t bRandomPacket = 0;
uint8_t bAnyPacket = 0;
uint16_t pack_size = 0;

void print_network_information(void);

int main(void)
{
	volatile int i;
	volatile int j,k;

	gpioInitialize();
	usartInitialize();
	timerInitialize();
#if 1
	// 20220824 taylor

	RCC_ClocksTypeDef w5100s_evb;
	RCC_GetClocksFreq(&w5100s_evb);

	printf("\r\n===================================\r\n");
	printf("System start.\r\n");
	printf("Compiled Date %s Time %s\r\n", __DATE__, __TIME__);
	printf("===================================\r\n");
	printf("SYSCLK_Frequency %ld MHz\r\n", w5100s_evb.SYSCLK_Frequency/1000000);
	printf("HCLK_Frequency %ld MHz\r\n", w5100s_evb.HCLK_Frequency/1000000);
	printf("PCLK1_Frequency %ld MHz\r\n", w5100s_evb.PCLK1_Frequency/1000000);
	printf("PCLK2_Frequency %ld MHz\r\n", w5100s_evb.PCLK2_Frequency/1000000);
	printf("ADCCLK_Frequency %ld MHz\r\n", w5100s_evb.ADCCLK_Frequency/1000000);
	printf("===================================\r\n\r\n");
#else
	printf("System start.\r\n");
#endif




#if _WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_
	// SPI method callback registration
	reg_wizchip_spi_cbfunc(spiReadByte, spiWriteByte);
#if 1
	// 20220825 taylor
	reg_wizchip_spiburst_cbfunc(spiReadBurst, spiWriteBurst);
#endif
	// CS function register
	reg_wizchip_cs_cbfunc(csEnable,csDisable);
#else
	// Indirect bus method callback registration
	//reg_wizchip_bus_cbfunc(busReadByte, busWriteByte);
#endif

#if _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_
	FSMCInitialize();
#else
	spiInitailize();
#endif
	resetAssert();
	delay(1);
	resetDeassert();
	delay(60);
	W5100SInitialze();
//	printf("\r\nCHIP Version: %02x\r\n", getVER());
//
//	//FSMCHighSpeed();
//	delay(10);
//	printf(" PHYMODE:%02x\r\n",getPHYSR0());
//	printf(" CHIP Version: %02x\r\n", getVER());
	wizchip_setnetinfo(&gWIZNETINFO);

	//printf("Register value after W5100S initialize!\r\n");
	//register_read();
	print_network_information();
	while(1)
    {
		loopback_tcps(0,ethBuf0,50000);
    }
}

void delay(unsigned int count)
{
	int temp;
	temp = count + TIM2_gettimer();
	while(temp > TIM2_gettimer()){}
}

void print_network_information(void)
{
	wizchip_getnetinfo(&gWIZNETINFO);
	printf("Mac address: %02x:%02x:%02x:%02x:%02x:%02x\n\r",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
	printf("IP address : %d.%d.%d.%d\n\r",gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
	printf("SM Mask	   : %d.%d.%d.%d\n\r",gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
	printf("Gate way   : %d.%d.%d.%d\n\r",gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
	printf("DNS Server : %d.%d.%d.%d\n\r",gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
}
