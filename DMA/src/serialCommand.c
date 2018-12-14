#include "stm32f10x_usart.h"
#include "HAL_Config.h"
#include "msgq.h"

//extern OS_MutexID usart_mutex;

/* for scanf() */
void USART1_IRQHandler(void)
{
	FlagStatus flagStatus;
	uint8_t recvData;
//	CoEnterISR();
	if((PRINTF_USART->SR & USART_FLAG_RXNE) != RESET)
	{
		queue_put(USART_ReceiveData(PRINTF_USART));
	}
	USART_ClearFlag(PRINTF_USART,USART_FLAG_RXNE);
//	CoExitISR();
}


/*Low layer read(input) function*/
__attribute__ ((used))
int _read(int file, char *ptr, int len)
{
	int i;
	int lenCnt = 0;
	(void)file;

	for(i = 0; i < len; i++)
	{
		while (queue_isEmpty() == 1){}
		// UART_GetChar is user's basic input function
		lenCnt++;
		queue_get(ptr);
		if(*ptr == 0x0d){
			*ptr = '\n';
			break;
		}
		ptr++;
	}
	return lenCnt;
}


/*Low layer write(output) function*/
__attribute__ ((used))
int _write(int file, char *ptr, int len)
{

     //user code example
	int i;
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
//	CoEnterMutexSection(usart_mutex);
	for(i = 0 ; i < len ; i++)
	{
		USART_SendData(PRINTF_USART, (uint8_t) *ptr);
		ptr++;
		/* Loop until the end of transmission */
		//while (USART_GetFlagStatus(PRINTF_USART, USART_FLAG_TC|USART_FLAG_TXE) == RESET){}
		//USART_ClearFlag(PRINTF_USART, USART_FLAG_TC|USART_FLAG_TXE);
		while (USART_GetFlagStatus(PRINTF_USART, USART_FLAG_TXE) == RESET){}
		USART_ClearFlag(PRINTF_USART,USART_FLAG_TXE);
		while (USART_GetFlagStatus(PRINTF_USART, USART_FLAG_TC) == RESET){}
		USART_ClearFlag(PRINTF_USART, USART_FLAG_TC);
	}
//	CoLeaveMutexSection(usart_mutex);
    return len;
}

char _getchar_nb()
{
  char c;
  if (queue_isEmpty()) return 0;
  queue_get(&c);
  return c;
}

char _getchar_b()
{
	  char c;
	  while (queue_isEmpty());
	  queue_get(&c);
	  return c;
}


uint16_t gets_echo(uint8_t* buf, uint16_t maxlen)
{
	volatile uint16_t i = 0;
	while(i < maxlen)
	{
		while (queue_isEmpty());
	    queue_get(&buf[i]);
	    if(buf[i] == 0x08)
	    {
	    	if(i)
	    	{
	    		_write(0,"\b",1);
	    		--i;
	    	}
	    }
	    else
	    {
	    	_write(0,&buf[i],1);
	    	if(buf[i] == '\n')
	    	{
	    		buf[++i] = 0;
	    		break;
	    	}
	    	i++;
	    }
	}
	return i;
}
