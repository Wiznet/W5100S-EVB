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
#include "../Libraries/ioLibrary_Driver/Internet/DHCP/dhcp.h"

wiz_NetInfo gWIZNETINFO = { .mac = {0x00,0x08,0xdc,0xff,0xff,0x89},
							.ip = {192,168,0,5},
							.sn = {255, 255, 255, 0},
							.gw = {192, 168, 0, 1},
							.dns = {168, 126, 63, 1},
//							.dhcp = NETINFO_STATIC};
                            .dhcp = NETINFO_DHCP};

#define ETH_MAX_BUF_SIZE	2048

unsigned char ethBuf0[ETH_MAX_BUF_SIZE];
unsigned char ethBuf1[ETH_MAX_BUF_SIZE];
unsigned char ethBuf2[ETH_MAX_BUF_SIZE];
unsigned char ethBuf3[ETH_MAX_BUF_SIZE];

uint8_t bLoopback = 1;
uint8_t bRandomPacket = 0;
uint8_t bAnyPacket = 0;
uint16_t pack_size = 0;

void dhcp_assign(void);
void dhcp_update(void);
void dhcp_conflict(void);
void print_network_information(void);

int main(void)
 {
	volatile int i;
	volatile int j,k;
	uint8_t tmp1, tmp2;
	uint32_t ret;
	uint8_t dhcp_retry = 0;
	uint8_t test_buf[ETH_MAX_BUF_SIZE];

	RCCInitialize();
	gpioInitialize();
	usartInitialize();
	timerInitialize();
	printf("System start.\r\n");




#if _WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_
	/* SPI method callback registration */
	reg_wizchip_spi_cbfunc(spiReadByte, spiWriteByte);
#ifdef SPI_DMA
	reg_wizchip_spiburst_cbfunc(spiReadBurst, spiWriteBurst);
#endif
	/* CS function register */
	reg_wizchip_cs_cbfunc(csEnable,csDisable);
#else
	/* Indirect bus method callback registration */
	reg_wizchip_bus_cbfunc(busReadByte, busWriteByte);
#ifdef BUS_DMA
	reg_wizchip_busburst_cbfunc(busReadBurst,busWriteBurst);

#endif
#endif
#if _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_
	FSMCInitialize();
#else
	spiInitailize();
#endif
	resetAssert();
	delay(10);
	resetDeassert();
	delay(10);
	W5100SInitialze();
	printf("\r\nCHIP Version: %02x\r\n", getVER());


	/* Wait Phy Link */
	while(1){
        ctlwizchip(CW_GET_PHYLINK, &tmp1 );
        ctlwizchip(CW_GET_PHYLINK, &tmp2 );
        if(tmp1==PHY_LINK_ON && tmp2==PHY_LINK_ON) break;
	}

	ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);

	/* DHCP Process */
    DHCP_init(0, test_buf);
    reg_dhcp_cbfunc(dhcp_assign, dhcp_update, dhcp_conflict);
    if (gWIZNETINFO.dhcp == NETINFO_DHCP) {       // CHEP DHCP
        printf("Start DHCP\r\n");
        while (1) {
            ret = DHCP_run();

            if (ret == DHCP_IP_LEASED) {
                printf("DHCP Success\r\n");
                break;
            }
            else if (ret == DHCP_FAILED) {
                dhcp_retry++;
            }

            if (dhcp_retry > 3) {
                printf("DHCP Fail\r\n");
                break;
            }
        }
    }
    ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);
	printf("Register value after W5100S initialize!\r\n");
	print_network_information();

	while(1)
    {
		loopback_tcps(0,ethBuf0,5000);
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


/**
 * @brief  The call back function of ip assign.
 * @note
 * @param  None
 * @retval None
 */
void dhcp_assign(void)
{
    getIPfromDHCP(gWIZNETINFO.ip);
    getGWfromDHCP(gWIZNETINFO.gw);
    getSNfromDHCP(gWIZNETINFO.sn);
    getDNSfromDHCP(gWIZNETINFO.dns);

    ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);
}

/**
 * @brief  The call back function of ip update.
 * @note
 * @param  None
 * @retval None
 */
void dhcp_update(void)
{
    ;
}

/**
 * @brief  The call back function of ip conflict.
 * @note
 * @param  None
 * @retval None
 */
void dhcp_conflict(void)
{
    ;
}
