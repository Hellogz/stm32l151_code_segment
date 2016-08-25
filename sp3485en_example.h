#ifndef __SP3485EN_EXAMPLE_H__
#define __SP3485EN_EXAMPLE_H__

#include "stm32l1xx.h"

#define RS485_MAX_BUFFER				255

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
- BaudRate = 9600 baud  
- Word Length = 8 Bits
- One Stop Bit
- No parity
- Hardware flow control disabled (RTS and CTS signals)
- Receive and transmit enabled
*/
#define USART_BAUDRATE                  9600
#define USART_WORDLENGTH                USART_WordLength_8b
#define USART_STOPBITS   				USART_StopBits_1
#define USART_PARITY                    USART_Parity_No
#define USART_HARDWAREFLOWCONTROL       USART_HardwareFlowControl_None
#define USART_MODE                      USART_Mode_Tx | USART_Mode_Rx

void rs485_usart_init(void);
void test_rs485(void);

#endif
