/*
Usart printf
*/
#include "stm32l1xx.h"
#include <stdio.h>

// Note! Keil->Option->Target->chose Use MicroLIB

#ifdef __GNUC__
    /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
    set to 'Yes') calls __io_putchar() */
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

// config defined
#define COM                             USART3
#define COM_CLK                         RCC_APB1Periph_USART3

#define COM_TX_GPIO_CKL                 RCC_AHBPeriph_GPIOB
#define COM_TX_PIN                      GPIO_Pin_10
#define COM_TX_GPIO_PORT                GPIOB
#define COM_TX_SOURCE                   GPIO_PinSource10
#define COM_TX_AF                       GPIO_AF_USART3

#define COM_RX_GPIO_CKL                 RCC_AHBPeriph_GPIOB
#define COM_RX_PIN                      GPIO_Pin_11
#define COM_RX_GPIO_PORT                GPIOB
#define COM_RX_SROUCE                   GPIO_PinSource11
#define COM_RX_AF                       GPIO_AF_USART3

/* USARTx configured as follow:
- BaudRate = 115200 baud  
- Word Length = 8 Bits
- One Stop Bit
- No parity
- Hardware flow control disabled (RTS and CTS signals)
- Receive and transmit enabled
*/
#define USART_BAUDRATE                  115200
#define USART_WORDLENGTH                USART_WordLength_8b
#define USART_STOPBITS   				USART_StopBits_1
#define USART_PARITY                    USART_Parity_No
#define USART_HARDWAREFLOWCONTROL       USART_HardwareFlowControl_None
#define USART_MODE                      USART_Mode_Rx | USART_Mode_Tx

/**
  * @brief  Config the USART.
  * @param  None
  * @retval None
  */
void com_config(void)
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

/*------------------ 2016/05/26 test ok ------------------*/
