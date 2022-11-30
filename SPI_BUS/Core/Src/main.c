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

#include "socket.h"
#include "Internet/DHCP/dhcp.h"
#include "wizchip_conf.h"
#include "W5100S/w5100s.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* DMA */
//#define USE_DMA

/* CHIP SETTING */
#define W5100S_CS_PIN			GPIO_PIN_7
#define W5100S_CS_PORT			GPIOD

#define RESET_W5100S_GPIO_Port	GPIOD
#define RESET_W5100S_Pin		GPIO_PIN_8

/* ETH */
#define ETH_MAX_BUF_SIZE		2048

#define SOCKET_DHCP 1
#define SOCKET_LOOP 2

#define PORT_LOOP   5000

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi2_rx;
DMA_HandleTypeDef hdma_spi2_tx;

UART_HandleTypeDef huart1;

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

uint8_t ethBuf[ETH_MAX_BUF_SIZE];

/* DHCP */




/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */

static void csEnable(void);
static void csDisable(void);

void print_network_information();


/* UART */
int _write(int fd, char *str, int len)
{
	for(int i=0; i<len; i++)
	{
		HAL_UART_Transmit(&huart1, (uint8_t *)&str[i], 1, 0xFFFF);
	}
	return len;
}

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
    /* Read version register */
    if (getVER() != 0x51) // W5100S
    {
        printf(" ACCESS ERR : VERSIONR != 0x51, read value = 0x%02x\n", getVER());
        while (1);
    }
}

void wizchip_initialize(void)
{
	uint8_t W5100S_AdrSet[2][4]= {{2,2,2,2},{2,2,2,2}};
    uint8_t tmp1, tmp2;
	intr_kind temp= IK_DEST_UNREACH;

	wizchip_reset();
	if (ctlwizchip(CW_INIT_WIZCHIP, (void*)W5100S_AdrSet) == -1)
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

void print_network_information()
{
	wizchip_getnetinfo(&gWIZNETINFO);

	printf("Mac address: %02x:%02x:%02x:%02x:%02x:%02x\n\r",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
	printf("IP address : %d.%d.%d.%d\n\r",gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
	printf("SM Mask	   : %d.%d.%d.%d\n\r",gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
	printf("Gate way   : %d.%d.%d.%d\n\r",gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
	printf("DNS Server : %d.%d.%d.%d\n\r",gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
}

/* SPI */
uint8_t spiReadByte(void)
{
	uint8_t readByte=0;
	uint8_t writeByte=0xFF;

	while(HAL_SPI_GetState(&hspi2)!=HAL_SPI_STATE_READY);
	HAL_SPI_TransmitReceive(&hspi2, &writeByte, &readByte, 1, 10);

	return readByte;
}

void spiWriteByte(uint8_t writeByte)
{
	uint8_t readByte=0;


	while(HAL_SPI_GetState(&hspi2)!=HAL_SPI_STATE_READY);
	HAL_SPI_TransmitReceive(&hspi2, &writeByte, &readByte, 1, 10);
}

/* SPI DMA */
uint8_t spiDmaReadByte(void)
{
	uint8_t readByte=0;
	uint8_t writeByte=0xFF;

	while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY &&
		  HAL_DMA_GetState(hspi2.hdmarx) != HAL_DMA_STATE_READY && HAL_DMA_GetState(hspi2.hdmatx) != HAL_DMA_STATE_READY);

	HAL_SPI_Receive_DMA(&hspi2, &readByte, 1);

	while (HAL_DMA_GetState(hspi2.hdmarx) == HAL_DMA_STATE_BUSY|| HAL_DMA_GetState(hspi2.hdmarx) == HAL_DMA_STATE_RESET);
	while (HAL_DMA_GetState(hspi2.hdmatx) == HAL_DMA_STATE_BUSY|| HAL_DMA_GetState(hspi2.hdmatx) == HAL_DMA_STATE_RESET);

	return readByte;

}

void spiDmaWriteByte(uint8_t writeByte)
{
	uint8_t readByte=0;

	while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY &&
		  HAL_DMA_GetState(hspi2.hdmarx) != HAL_DMA_STATE_READY && HAL_DMA_GetState(hspi2.hdmatx) != HAL_DMA_STATE_READY);

	HAL_SPI_Transmit_DMA(&hspi2, &writeByte, 1);

	while (HAL_DMA_GetState(hspi2.hdmarx) == HAL_DMA_STATE_BUSY|| HAL_DMA_GetState(hspi2.hdmarx) == HAL_DMA_STATE_RESET);
	while (HAL_DMA_GetState(hspi2.hdmatx) == HAL_DMA_STATE_BUSY|| HAL_DMA_GetState(hspi2.hdmatx) == HAL_DMA_STATE_RESET);

	return;
}

void spiReadBurst(uint8_t* pBuf, uint16_t len)
{
	while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY &&
		  HAL_DMA_GetState(hspi2.hdmarx) != HAL_DMA_STATE_READY && HAL_DMA_GetState(hspi2.hdmatx) != HAL_DMA_STATE_READY);

	HAL_SPI_Receive_DMA(&hspi2, pBuf, len);

	while (HAL_DMA_GetState(hspi2.hdmarx) == HAL_DMA_STATE_BUSY|| HAL_DMA_GetState(hspi2.hdmarx) == HAL_DMA_STATE_RESET);
	while (HAL_DMA_GetState(hspi2.hdmatx) == HAL_DMA_STATE_BUSY|| HAL_DMA_GetState(hspi2.hdmatx) == HAL_DMA_STATE_RESET);

	return;
}

void spiWriteBurst(uint8_t* pBuf, uint16_t len)
{
	while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY &&
		  HAL_DMA_GetState(hspi2.hdmarx) != HAL_DMA_STATE_READY && HAL_DMA_GetState(hspi2.hdmatx) != HAL_DMA_STATE_READY);

	HAL_SPI_Transmit_DMA(&hspi2, pBuf, len);

	while (HAL_DMA_GetState(hspi2.hdmarx) == HAL_DMA_STATE_BUSY|| HAL_DMA_GetState(hspi2.hdmarx) == HAL_DMA_STATE_RESET);
	while (HAL_DMA_GetState(hspi2.hdmatx) == HAL_DMA_STATE_BUSY|| HAL_DMA_GetState(hspi2.hdmatx) == HAL_DMA_STATE_RESET);

	return;
}

/* SPI CS CTRL */
static void csEnable(void)
{
	HAL_GPIO_WritePin(W5100S_CS_PORT, W5100S_CS_PIN, GPIO_PIN_RESET);
}

static void csDisable(void)
{
	HAL_GPIO_WritePin(W5100S_CS_PORT, W5100S_CS_PIN, GPIO_PIN_SET);
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
    DHCP_init(SOCKET_DHCP, ethBuf);
    reg_dhcp_cbfunc(wizchip_dhcp_assign, wizchip_dhcp_assign, wizchip_dhcp_conflict);
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
 * @brief  The call back function of ip assign.
 * @note
 * @param  None
 * @retval None
 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
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
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */

  printf("\r\n system start (%d Hz)\r\n", HAL_RCC_GetSysClockFreq());

  reg_wizchip_cs_cbfunc(csEnable,csDisable);// CS function register

#ifdef USE_DMA
  reg_wizchip_spi_cbfunc(spiDmaReadByte, spiDmaWriteByte);// SPI method callback registration
  reg_wizchip_spiburst_cbfunc(spiReadBurst, spiWriteBurst);// use DMA
#else
  reg_wizchip_spi_cbfunc(spiReadByte, spiWriteByte);// SPI method callback registration
#endif


  wizchip_initialize();
  if (gWIZNETINFO.dhcp == NETINFO_DHCP) // DHCP
  {
	  wizchip_dhcp_init();

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
	  /* Assigned IP through DHCP */
	  if (gWIZNETINFO.dhcp == NETINFO_DHCP)
		  DHCP_run();

	  loopback_tcps(SOCKET_LOOP, ethBuf, PORT_LOOP);

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
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD8 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
