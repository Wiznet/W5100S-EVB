/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "Application/loopback/loopback.h"
#include "socket.h"
#include "Internet/DHCP/dhcp.h"
#include "wizchip_conf.h"
#include "W5100S/w5100s.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* DMA */
#if 1
#define USE_DMA
#endif

#if 0
#define BUS_DMA_INT
#else
#define BUS_DMA_POL
#endif

/* CHIP SETTING */
#define RESET_W5100S_GPIO_Port	GPIOD
#define RESET_W5100S_Pin		GPIO_PIN_8

/* ETH */
#define ETH_MAX_BUF_SIZE		2048

#define SOCKET_DHCP 3

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

DMA_HandleTypeDef hdma_memtomem_dma1_channel4;
DMA_HandleTypeDef hdma_memtomem_dma1_channel5;
SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */

/* NET */
wiz_NetInfo gWIZNETINFO = {
		.mac = {0x00, 0x08, 0xdc, 0x6f, 0x00, 0x8a},
		.ip = {192, 168, 11, 109},
		.sn = {255, 255, 255, 0},
		.gw = {192, 168, 11, 1},
		.dns = {8, 8, 8, 8},
		.dhcp = NETINFO_DHCP
};

uint8_t ethBuf0[ETH_MAX_BUF_SIZE];
uint8_t ethBuf1[ETH_MAX_BUF_SIZE];
uint8_t ethBuf2[ETH_MAX_BUF_SIZE];
uint8_t ethBuf3[ETH_MAX_BUF_SIZE];

uint8_t dma_ch4_comp = 0;
uint8_t dma_ch5_comp = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_FSMC_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void print_network_information();
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
static void wizchip_dhcp_assign(void);
static void wizchip_dhcp_conflict(void);
void wizchip_dhcp_init(void);

void busDmaWriteByte(uint32_t addr, iodata_t data);
iodata_t busDmaReadByte(uint32_t addr);
void busDmaWriteBurst(uint32_t addr, uint8_t* pBuf ,uint32_t len,uint8_t addr_inc);
void busDmaReadBurst(uint32_t addr,uint8_t* pBuf, uint32_t len,uint8_t addr_inc);
void XferCpltCallback_ch4(DMA_HandleTypeDef *hdma);
void XferCpltCallback_ch5(DMA_HandleTypeDef *hdma);

void wizchip_reset();
void wizchip_check(void);
void wizchip_initialize(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint8_t svr_ipv4[4] = {192, 168, 177, 235};
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_FSMC_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  printf("\r\n");
  printf("===========================================================\r\n");
  printf("Compiled @ %s %s\r\n", __DATE__, __TIME__);
  printf("W5100S-EVB FSMC\r\n");
  printf("System start (%ld Hz)\r\n", HAL_RCC_GetSysClockFreq());
  printf("===========================================================\r\n");

  HAL_TIM_Base_Start_IT(&htim2);

#ifdef USE_DMA
  reg_wizchip_bus_cbfunc(busDmaReadByte, busDmaWriteByte);
  reg_wizchip_busbuf_cbfunc(busDmaReadBurst, busDmaWriteBurst);
#else
  reg_wizchip_bus_cbfunc(0, 0);
  reg_wizchip_busbuf_cbfunc(0, 0);
#endif

  wizchip_initialize();

  wizchip_setnetinfo(&gWIZNETINFO);

  printf("Network Information\r\n");
  print_network_information();

  if (gWIZNETINFO.dhcp == NETINFO_DHCP) // DHCP
  {
    printf("DHCP init()\r\n");
	  wizchip_dhcp_init();

    printf("DHCP run()\r\n");
	  while (DHCP_run() != DHCP_IP_LEASED)
	      wiz_delay_ms(1000);
  }
  else // static
  {
	  ctlnetwork(CN_SET_NETINFO, &gWIZNETINFO);
	  printf("\r\n----------STATIC Net Information--------------\r\n");
    print_network_information();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
#if 1
	  loopback_udps(0, ethBuf0, 50000);
#endif

#if 0
	  loopback_tcpc(1, ethBuf1, svr_ipv4, 50001);
#endif

#if 1
	  loopback_tcps(2, ethBuf2, 50002);
#endif

	  /* Assigned IP through DHCP */
    if (gWIZNETINFO.dhcp == NETINFO_DHCP)
    {
		  DHCP_run();
    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 1000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 6999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  * Configure DMA for memory to memory transfers
  *   hdma_memtomem_dma1_channel4
  *   hdma_memtomem_dma1_channel5
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Configure DMA request hdma_memtomem_dma1_channel4 on DMA1_Channel4 */
  hdma_memtomem_dma1_channel4.Instance = DMA1_Channel4;
  hdma_memtomem_dma1_channel4.Init.Direction = DMA_MEMORY_TO_MEMORY;
  hdma_memtomem_dma1_channel4.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_memtomem_dma1_channel4.Init.MemInc = DMA_MINC_ENABLE;
  hdma_memtomem_dma1_channel4.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_memtomem_dma1_channel4.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_memtomem_dma1_channel4.Init.Mode = DMA_NORMAL;
  hdma_memtomem_dma1_channel4.Init.Priority = DMA_PRIORITY_HIGH;
  if (HAL_DMA_Init(&hdma_memtomem_dma1_channel4) != HAL_OK)
  {
    Error_Handler( );
  }

  /* Configure DMA request hdma_memtomem_dma1_channel5 on DMA1_Channel5 */
  hdma_memtomem_dma1_channel5.Instance = DMA1_Channel5;
  hdma_memtomem_dma1_channel5.Init.Direction = DMA_MEMORY_TO_MEMORY;
  hdma_memtomem_dma1_channel5.Init.PeriphInc = DMA_PINC_ENABLE;
  hdma_memtomem_dma1_channel5.Init.MemInc = DMA_MINC_DISABLE;
  hdma_memtomem_dma1_channel5.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_memtomem_dma1_channel5.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_memtomem_dma1_channel5.Init.Mode = DMA_NORMAL;
  hdma_memtomem_dma1_channel5.Init.Priority = DMA_PRIORITY_HIGH;
  if (HAL_DMA_Init(&hdma_memtomem_dma1_channel5) != HAL_OK)
  {
    Error_Handler( );
  }

  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin : PD8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* FSMC initialization function */
static void MX_FSMC_Init(void)
{

  /* USER CODE BEGIN FSMC_Init 0 */

  /* USER CODE END FSMC_Init 0 */

  FSMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FSMC_Init 1 */

  /* USER CODE END FSMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_ENABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_PSRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_8;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  /* Timing */
  Timing.AddressSetupTime = 0x03;
  Timing.AddressHoldTime = 0x01;
  Timing.DataSetupTime = 0x08;
  Timing.BusTurnAroundDuration = 0;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /** Disconnect NADV
  */

  __HAL_AFIO_FSMCNADV_DISCONNECTED();

  /* USER CODE BEGIN FSMC_Init 2 */
  HAL_NVIC_SetPriority(FSMC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(FSMC_IRQn);
  /* USER CODE END FSMC_Init 2 */
}

/* USER CODE BEGIN 4 */

/* UART */
int _write(int fd, char *str, int len)
{
	for(int i=0; i<len; i++)
	{
		HAL_UART_Transmit(&huart1, (uint8_t *)&str[i], 1, 0xFFFF);
	}
	return len;
}

void print_network_information(void)
{
	wizchip_getnetinfo(&gWIZNETINFO);

	printf("Mac address: %02x:%02x:%02x:%02x:%02x:%02x\r\n",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
	printf("IP address : %d.%d.%d.%d\r\n",gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
	printf("SM Mask	   : %d.%d.%d.%d\r\n",gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
	printf("Gate way   : %d.%d.%d.%d\r\n",gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
	printf("DNS Server : %d.%d.%d.%d\r\n",gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	DHCP_time_handler();
}

/* DHCP */
static void wizchip_dhcp_assign(void)
{
    getIPfromDHCP(gWIZNETINFO.ip);
    getGWfromDHCP(gWIZNETINFO.gw);
    getSNfromDHCP(gWIZNETINFO.sn);
    getDNSfromDHCP(gWIZNETINFO.dns);
    gWIZNETINFO.dhcp = NETINFO_DHCP;

    ctlnetwork(CN_SET_NETINFO, &gWIZNETINFO);
    printf("\r\n----------DHCP Net Information--------------\r\n");
    print_network_information();
}

static void wizchip_dhcp_conflict(void)
{
	printf("wizchip_dhcp_conflict\r\n");
}

void wizchip_dhcp_init(void)
{
    DHCP_init(SOCKET_DHCP, ethBuf3);
    reg_dhcp_cbfunc(wizchip_dhcp_assign, wizchip_dhcp_assign, wizchip_dhcp_conflict);
}

#ifdef USE_DMA
/* BUS_DMA */

void busDmaWriteByte(uint32_t addr, iodata_t data)
{
#ifdef BUS_DMA_INT
	hdma_memtomem_dma1_channel4.XferCpltCallback = &XferCpltCallback_ch4;
	HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel4, &data, (uint32_t)addr, 1);
	while(dma_ch4_comp != 1);
	dma_ch4_comp = 0;
#elif defined BUS_DMA_POL
	HAL_DMA_Start(&hdma_memtomem_dma1_channel4, (uint32_t)&data, (uint32_t)addr, 1);
	HAL_StatusTypeDef status;
	status = HAL_DMA_PollForTransfer(&hdma_memtomem_dma1_channel4, HAL_DMA_FULL_TRANSFER, 100);
	if(status != HAL_OK)
	{
		printf("status = %d\r\n", status);
	}
#endif
}

iodata_t busDmaReadByte(uint32_t addr)
{
	iodata_t ret;

#ifdef BUS_DMA_INT
	hdma_memtomem_dma1_channel5.XferCpltCallback = &XferCpltCallback_ch5;
	HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel5, (uint32_t)addr, &ret, 1);
	while(dma_ch5_comp != 1);
	dma_ch5_comp = 0;
#elif defined BUS_DMA_POL
	HAL_DMA_Start(&hdma_memtomem_dma1_channel5, (uint32_t)addr, (uint32_t)&ret, 1);
	HAL_StatusTypeDef status;
	status = HAL_DMA_PollForTransfer(&hdma_memtomem_dma1_channel5, HAL_DMA_FULL_TRANSFER, 100);
	if(status != HAL_OK)
	{
		printf("status = %d\r\n", status);
	}
#endif
	return ret;
}

void busDmaWriteBurst(uint32_t addr, uint8_t* pBuf ,uint32_t len,uint8_t addr_inc)
{
	HAL_DMA_DeInit(&hdma_memtomem_dma1_channel4);

	hdma_memtomem_dma1_channel4.Init.PeriphInc = DMA_PINC_ENABLE;

	if(addr_inc)
	{
		hdma_memtomem_dma1_channel4.Init.MemInc = DMA_MINC_ENABLE;
	}
	else
	{
		hdma_memtomem_dma1_channel4.Init.MemInc = DMA_MINC_DISABLE;
	}

	hdma_memtomem_dma1_channel4.Instance = DMA1_Channel4;
	hdma_memtomem_dma1_channel4.Init.Direction = DMA_MEMORY_TO_MEMORY;
	//hdma_memtomem_dma1_channel4.Init.PeriphInc = DMA_PINC_DISABLE;
	//hdma_memtomem_dma1_channel4.Init.MemInc = DMA_MINC_ENABLE;
	hdma_memtomem_dma1_channel4.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_memtomem_dma1_channel4.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_memtomem_dma1_channel4.Init.Mode = DMA_NORMAL;
	hdma_memtomem_dma1_channel4.Init.Priority = DMA_PRIORITY_HIGH;

#ifdef BUS_DMA_INT
	hdma_memtomem_dma1_channel4.XferCpltCallback = &XferCpltCallback_ch4;
#endif

	if (HAL_DMA_Init(&hdma_memtomem_dma1_channel4) != HAL_OK)
	{
		Error_Handler( );
	}

	while (HAL_DMA_GetState(&hdma_memtomem_dma1_channel4) != HAL_DMA_STATE_READY);
#ifdef BUS_DMA_POL
	HAL_DMA_Start(&hdma_memtomem_dma1_channel4, (uint32_t)pBuf, (uint32_t)addr, len);
	HAL_StatusTypeDef status;
	status = HAL_DMA_PollForTransfer(&hdma_memtomem_dma1_channel4, HAL_DMA_FULL_TRANSFER, 100);
	if(status != HAL_OK)
	{
		printf("status = %d\r\n", status);
	}
#elif defined BUS_DMA_INT
	HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel4, pBuf, (uint32_t)addr, len);
	while(dma_ch4_comp != 1);
	dma_ch4_comp = 0;
#endif
}

void busDmaReadBurst(uint32_t addr,uint8_t* pBuf, uint32_t len,uint8_t addr_inc)
{
	HAL_DMA_DeInit(&hdma_memtomem_dma1_channel5);

	hdma_memtomem_dma1_channel5.Instance = DMA1_Channel5;
	hdma_memtomem_dma1_channel5.Init.Direction = DMA_MEMORY_TO_MEMORY;
	hdma_memtomem_dma1_channel5.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_memtomem_dma1_channel5.Init.MemInc = DMA_MINC_ENABLE;
	hdma_memtomem_dma1_channel5.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_memtomem_dma1_channel5.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_memtomem_dma1_channel5.Init.Mode = DMA_NORMAL;
	hdma_memtomem_dma1_channel5.Init.Priority = DMA_PRIORITY_HIGH;

#ifdef BUS_DMA_INT
	hdma_memtomem_dma1_channel5.XferCpltCallback = &XferCpltCallback_ch5;
#endif

	if (HAL_DMA_Init(&hdma_memtomem_dma1_channel5) != HAL_OK)
	{
		Error_Handler( );
	}

	while (HAL_DMA_GetState(&hdma_memtomem_dma1_channel5) != HAL_DMA_STATE_READY);
#ifdef BUS_DMA_POL
	HAL_DMA_Start(&hdma_memtomem_dma1_channel5, (uint32_t)addr, (uint32_t)pBuf, len);
	HAL_StatusTypeDef status;
	status = HAL_DMA_PollForTransfer(&hdma_memtomem_dma1_channel5, HAL_DMA_FULL_TRANSFER, 100);
	if(status != HAL_OK)
	{
		printf("status = %d\r\n", status);
	}
#elif defined BUS_DMA_INT
	HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel5, (uint32_t)addr, pBuf, len);
	while(dma_ch5_comp != 1);
	dma_ch5_comp = 0;
#endif
}

void XferCpltCallback_ch4(DMA_HandleTypeDef *hdma)
{
	dma_ch4_comp = 1;
}

void XferCpltCallback_ch5(DMA_HandleTypeDef *hdma)
{
	dma_ch5_comp = 1;
}
#endif

/* CHIP INIT */
void wizchip_reset()
{
	HAL_GPIO_WritePin(RESET_W5100S_GPIO_Port, RESET_W5100S_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(RESET_W5100S_GPIO_Port, RESET_W5100S_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
}

void wizchip_check(void)
{
    if (getVER() != 0x51) // W5100S
    {
        printf(" ACCESS ERR : VERSIONR != 0x51, read value = 0x%02x\n", getVER());
        while (1);
    }
}

void wizchip_initialize(void)
{
	uint8_t W5100S_AdrSet[2][4] = {{2,2,2,2},{2,2,2,2}};
	uint8_t tmp1, tmp2;
	intr_kind temp= IK_DEST_UNREACH;

	wizchip_reset();
	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)W5100S_AdrSet) == -1)
		printf(">>>>W5100s memory initialization failed\r\n");

	if(ctlwizchip(CW_SET_INTRMASK,&temp) == -1)
		printf("W5100S interrupt\r\n");

	wizchip_check();
  while(1)
	{
		ctlwizchip(CW_GET_PHYLINK, &tmp1 );
		ctlwizchip(CW_GET_PHYLINK, &tmp2 );
		if(tmp1==PHY_LINK_ON && tmp2==PHY_LINK_ON) break;
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
