/*
STM32L151 + SP3485 Example
author: Hellogz 2016/8/25
*/

#include "sp3485en_example.h"


uint8_t rs485_buffer[RS485_MAX_BUFFER] = {0};
uint8_t rs485_buffer_size = 0;
bool rs485_received_flag = false;

/**
  * @brief  Config the USART.
  * @param  None
  * @retval None
  */
void rs485_usart_init(void)
{
	USART_InitTypeDef 		USART_InitStructure;
	GPIO_InitTypeDef 		GPIO_InitStructure;
	NVIC_InitTypeDef 		NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(COM_TX_GPIO_CKL | COM_RX_GPIO_CKL, ENABLE);

	/* Connect PXx to USARTx_Tx */
	GPIO_PinAFConfig(COM_TX_GPIO_PORT, COM_TX_SOURCE, COM_TX_AF);

	/* Connect PXx to USARTx_Rx */
	GPIO_PinAFConfig(COM_RX_GPIO_PORT, COM_RX_SROUCE, COM_RX_AF);

	/* Configure USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = COM_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(COM_TX_GPIO_PORT, &GPIO_InitStructure);

	/* Configure USART Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = COM_RX_PIN;
	GPIO_Init(COM_TX_GPIO_PORT, &GPIO_InitStructure);
	
	/* Enable UART clock */
	RCC_APB1PeriphClockCmd(COM_CLK, ENABLE);

	USART_InitStructure.USART_BaudRate = USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WORDLENGTH;
	USART_InitStructure.USART_StopBits = USART_STOPBITS;
	USART_InitStructure.USART_Parity = USART_PARITY;
	USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL;
	USART_InitStructure.USART_Mode = USART_MODE;

	/* USART configuration */
	USART_Init(COM, &USART_InitStructure);

	/* Enable USART */
	USART_Cmd(COM, ENABLE);
	
	USART_ITConfig(COM, USART_IT_RXNE, ENABLE);
}

uint8_t rs485_send(uint8_t ch)
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(COM, (uint8_t) ch);

	/* Loop until transmit data register is empty */
	while (USART_GetFlagStatus(COM, USART_FLAG_TXE) == RESET)
	{}
	return ch;
}

void USART3_IRQHandler( void )
{
	uint8_t data;

	if( USART_GetITStatus( COM, USART_IT_ORE_RX ) != RESET )
	{
		USART_ReceiveData( COM );
	}

	if( USART_GetITStatus( COM, USART_IT_RXNE ) != RESET )
	{    
		data = USART_ReceiveData( COM );
		if(rs485_buffer_size == RS485_MAX_BUFFER)
		{
			rs485_buffer_size = 0;
		}
		rs485_buffer[rs485_buffer_size++] = data;
		if(data == 0xEF)
		{
			rs485_received_flag = true;
		}
	}
}

void test_rs485(void)
{
	uint8_t buf[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	
	rs485_usart_init();

	for(uint8_t i = 0; i < 10; i++)
	{
		rs485_send(buf[i]);		// test send data
	}

	while(1)
	{
		if(rs485_received_flag)	// received data
		{
			for(uint8_t i = 0; i < rs485_buffer_size; i++)
			{
				rs485_send(rs485_buffer[i]);
			}
			rs485_received_flag = false;
			rs485_buffer_size = 0;
		}
	}
	
}
