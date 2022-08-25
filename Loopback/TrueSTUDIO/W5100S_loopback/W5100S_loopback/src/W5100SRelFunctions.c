#include "HAL_Config.h"
#include "W5100SRelFunctions.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"

DMA_InitTypeDef		DMA_RX_InitStructure, DMA_TX_InitStructure;

void W5100SInitialze(void)
{
	intr_kind temp;
	unsigned char W5100S_AdrSet[2][4] = {{2,2,2,2},{2,2,2,2}};
	/*
	 */
	temp = IK_DEST_UNREACH;

	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)W5100S_AdrSet) == -1)
	{
		printf("W5100S initialized fail.\r\n");
	}

	if(ctlwizchip(CW_SET_INTRMASK,&temp) == -1)
	{
		printf("W5100S interrupt\r\n");
	}
	//printf("interrupt mask: %02x\r\n",getIMR());

//	do{//check phy status.
//		if(ctlwizchip(CW_GET_PHYLINK,(void*)&temp) == -1){
//			printf("Unknown PHY link status.\r\n");
//		}
//	}while(temp == PHY_LINK_OFF);
}

uint8_t spiReadByte(void)
{
	while (SPI_I2S_GetFlagStatus(W5100S_SPI, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(W5100S_SPI, 0xff);
	while (SPI_I2S_GetFlagStatus(W5100S_SPI, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(W5100S_SPI);
}

void spiWriteByte(uint8_t byte)
{
	while (SPI_I2S_GetFlagStatus(W5100S_SPI, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(W5100S_SPI, byte);
	while (SPI_I2S_GetFlagStatus(W5100S_SPI, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(W5100S_SPI);
}

uint8_t spiReadBurst(uint8_t* pBuf, uint16_t len)
{
	unsigned char tempbuf =0xff;
	DMA_TX_InitStructure.DMA_BufferSize = len;
	DMA_TX_InitStructure.DMA_MemoryBaseAddr = &tempbuf;
	//DMA_TX_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_Init(W5100S_DMA_CHANNEL_TX, &DMA_TX_InitStructure);

	DMA_RX_InitStructure.DMA_BufferSize = len;
	DMA_RX_InitStructure.DMA_MemoryBaseAddr = pBuf;
	DMA_Init(W5100S_DMA_CHANNEL_RX, &DMA_RX_InitStructure);

	/* Enable SPI Rx/Tx DMA Request*/
	DMA_Cmd(W5100S_DMA_CHANNEL_RX, ENABLE);
	DMA_Cmd(W5100S_DMA_CHANNEL_TX, ENABLE);

	/* Waiting for the end of Data Transfer */
	while(DMA_GetFlagStatus(DMA_TX_FLAG) == RESET);
	while(DMA_GetFlagStatus(DMA_RX_FLAG) == RESET);

	DMA_ClearFlag(DMA_TX_FLAG | DMA_RX_FLAG);

	DMA_Cmd(W5100S_DMA_CHANNEL_TX, DISABLE);
	DMA_Cmd(W5100S_DMA_CHANNEL_RX, DISABLE);
}

void spiWriteBurst(uint8_t* pBuf, uint16_t len)
{
	unsigned char tempbuf;
	DMA_TX_InitStructure.DMA_BufferSize = len;
	DMA_TX_InitStructure.DMA_MemoryBaseAddr = pBuf;
	DMA_Init(W5100S_DMA_CHANNEL_TX, &DMA_TX_InitStructure);

	DMA_RX_InitStructure.DMA_BufferSize = 1;
	DMA_RX_InitStructure.DMA_MemoryBaseAddr = &tempbuf;
	DMA_RX_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_Init(W5100S_DMA_CHANNEL_RX, &DMA_RX_InitStructure);

	/* Enable SPI Rx/Tx DMA Request*/
	DMA_Cmd(W5100S_DMA_CHANNEL_RX, ENABLE);
	DMA_Cmd(W5100S_DMA_CHANNEL_TX, ENABLE);

	/* Waiting for the end of Data Transfer */
	while(DMA_GetFlagStatus(DMA_TX_FLAG) == RESET);
	while(DMA_GetFlagStatus(DMA_RX_FLAG) == RESET);

	DMA_ClearFlag(DMA_TX_FLAG | DMA_RX_FLAG);

	DMA_Cmd(W5100S_DMA_CHANNEL_TX, DISABLE);
	DMA_Cmd(W5100S_DMA_CHANNEL_RX, DISABLE);
}

//(*bus_wb)(uint32_t addr, iodata_t wb);
void busWriteByte(uint32_t addr, iodata_t data)
{
	(*((volatile uint8_t*)(W5100SAddress+1))) = (uint8_t)((addr &0xFF00)>>8);
	(*((volatile uint8_t*)(W5100SAddress+2))) = (uint8_t)((addr) & 0x00FF);
	(*((volatile uint8_t*)(W5100SAddress+3))) = data;
}

//iodata_t (*bus_rb)(uint32_t addr);
iodata_t busReadByte(uint32_t addr)
{
	(*((volatile uint8_t*)(W5100SAddress+1))) = (uint8_t)((addr &0xFF00)>>8);
	(*((volatile uint8_t*)(W5100SAddress+2))) = (uint8_t)((addr) & 0x00FF);
	return  (*((volatile uint8_t*)(W5100SAddress+3)));
}

inline void csEnable(void)
{
	GPIO_ResetBits(W5100S_CS_PORT, W5100S_CS_PIN);
}

inline void csDisable(void)
{
	GPIO_SetBits(W5100S_CS_PORT, W5100S_CS_PIN);
}

inline void resetAssert(void)
{
	GPIO_ResetBits(W5100S_RESET_PORT, W5100S_RESET_PIN);
}

inline void resetDeassert(void)
{
	GPIO_SetBits(W5100S_RESET_PORT, W5100S_RESET_PIN);
}

void W5100SReset(void)
{
	int i,j,k;
	k=0;
	GPIO_ResetBits(W5100S_RESET_PORT,W5100S_RESET_PIN);
	CoTickDelay(10);
	GPIO_SetBits(W5100S_RESET_PORT,W5100S_RESET_PIN);
}
