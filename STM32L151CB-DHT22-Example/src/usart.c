#include "usart.h"

/**
  * @brief  Config the USART.
  * @param  None
  * @retval None
  */
void debug_printf_com_config(void)
{
    USART_InitTypeDef 	USART_InitStructure;
    GPIO_InitTypeDef 	GPIO_InitStructure;

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
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(COM, (uint8_t) ch);

	/* Loop until transmit data register is empty */
	while (USART_GetFlagStatus(COM, USART_FLAG_TXE) == RESET)
	{}
	return ch;
}

