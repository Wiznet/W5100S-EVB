#include "HAL_Config.h"

#include "HALInit.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_fsmc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include <stdio.h>


volatile unsigned long globalTimer = 0;

void HardFault_Handler(void)
{
	printf("Hard Fault!!!\r\n");
	while(1);
}

void MemManage_Handler(void)
{
	printf("Mem Manage!!!\r\n");
	while(1);
}

void NMI_Handler(void)
{
	printf("NMI Handler!!!\r\n");
	while(1);
}

void BusFault_Handler(void)
{
	printf("Bus Fault!!!\r\n");
	while(1);
}

void UsageFault_Handler(void)
{
	printf("Bus Fault!!!\r\n");
	while(1);
}


void timerInitialize(void)
{
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

        TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
        TIM_TimeBaseInitStruct.TIM_Period = 8400-1;
        TIM_TimeBaseInitStruct.TIM_Prescaler = 1000-1;
        TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
        TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
        TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
        TIM_SetAutoreload(TIM2,8400-1);
        TIM_Cmd(TIM2,ENABLE);

        NVIC_InitTypeDef   NVIC_InitStructure;

        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void){
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
        globalTimer++;
}


void TIM2_settimer(void)
{
	TIM_Cmd(TIM2,DISABLE);
	globalTimer = 0;
	TIM_Cmd(TIM2,ENABLE);
}

unsigned long TIM2_gettimer(void)
{
	return globalTimer;
}

void clockConfiguration(void)
{

}

void gpioInitialize(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/*GPIO For USART initialize*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* Configure USART Tx */
	GPIO_InitStructure.GPIO_Pin = PRINTF_USART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(PRINTF_USART_TX_PORT, &GPIO_InitStructure);

	/* Configure USART Rx */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = PRINTF_USART_RX_PIN;

	GPIO_Init(PRINTF_USART_RX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* GPIO for W5100S initialize */
	RCC_APB2PeriphClockCmd(W5100S_GPIO_RCC, ENABLE);

	/*For SPI*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = W5100S_SPI_SCK_PIN | W5100S_SPI_MOSI_PIN | W5100S_SPI_MISO_PIN;
	GPIO_Init(W5100S_SPI_PORT, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	/*For CS*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = W5100S_CS_PIN;
	GPIO_Init(W5100S_CS_PORT, &GPIO_InitStructure);

	/*For Reset*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = W5100S_RESET_PIN;
	GPIO_Init(W5100S_RESET_PORT, &GPIO_InitStructure);
/*
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);

	  GPIO_WriteBit(GPIOC,GPIO_Pin_6,SET);
*/
	/*For W5100S interrupt*/

	/* Enable SYSCFG clock */
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

//	/* Connect EXTI Line14 to PA14 pin */
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource14);
//
//	/* Configure EXTI Line0 */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
//
//	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//
//	/*For Interrupt GPIO*/
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_Pin = W5100S_INT_PIN;
//	GPIO_Init(W5100S_INT_PORT, &GPIO_InitStructure);
}

void usartInitialize(void)
{
	RCC_APB2PeriphClockCmd(PRINTF_USART_RCC, ENABLE);

	USART_InitTypeDef USART_InitStructure;;

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* USART configuration */
	USART_Init(PRINTF_USART, &USART_InitStructure);

	/* USART interrupt enable */
	USART_ITConfig(PRINTF_USART, USART_IT_RXNE, ENABLE);
	USART_ITConfig(PRINTF_USART, USART_IT_TXE, DISABLE);


	/* Enable USART */
	USART_Cmd(PRINTF_USART, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void spiInitailize(void)
{
	/*W5100S SPI initialize*/
	RCC_APB1PeriphClockCmd(W5100S_SPI_RCC, ENABLE);

	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	/* Initializes the SPI communication */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;

	SPI_Init(W5100S_SPI, &SPI_InitStructure);

	SPI_Cmd(W5100S_SPI,ENABLE);
}

void FSMCInitialize(void)
{
/*
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;
*/
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);

	/* Enable FSMC clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	//DATA |PD14|PD15|PD0|PD1|PE7|PE8|PE9|PE10|
	/* GPIOD configuration for data 0~3 and */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* GPIOE configuration for fsmc data d4~d7*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 ;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* GPIOB configuration for fsmc nadv */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* NOE and NWE configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* NE1 configuration */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);
	/* NBL0, NBL1 configuration */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);

#if 0
	/*-- FSMC Configuration ------------------------------------------------------*/
	p.FSMC_AddressSetupTime = 0x0;
	p.FSMC_AddressHoldTime = 0;
	p.FSMC_DataSetupTime = 0x10;
	p.FSMC_BusTurnAroundDuration = 0;
	p.FSMC_CLKDivision = 0x00;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_A;

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	/*!< Enable FSMC Bank1_SRAM1 Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
#endif
	//FSMCLowSpeed();
	FSMCHighSpeed();
}

void FSMCLowSpeed()
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;

	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, DISABLE);

	p.FSMC_AddressSetupTime = 0x0a;
	p.FSMC_AddressHoldTime = 00;
	p.FSMC_DataSetupTime = 0x1a;
	p.FSMC_BusTurnAroundDuration = 5;
	p.FSMC_CLKDivision = 0x00;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_B;

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Enable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	/*!< Enable FSMC Bank1_SRAM1 Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

void FSMCHighSpeed()
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;

	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, DISABLE);

	p.FSMC_AddressSetupTime = 0x03;
	p.FSMC_AddressHoldTime = 0x01;
	p.FSMC_DataSetupTime = 0x08;
	p.FSMC_BusTurnAroundDuration = 0;
	p.FSMC_CLKDivision = 0x00;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_B;

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Enable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	/*!< Enable FSMC Bank1_SRAM1 Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

