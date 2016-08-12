#ifndef __DHT22__
#define __DHT22__

#include "stm32l1xx.h"
#include "tick_delay.h"
#include "usart.h"

/* Port and pin with DHT22 sensor*/
#define MAX_TICS 					10000
#define DHT22_OK 					0
#define DHT22_NO_CONN 				1
#define DHT22_CS_ERROR 				2
#define DHT22_PORT 					GPIOA
#define DHT22_GPIO_CLOCK            RCC_AHBPeriph_GPIOA
#define DHT22_PIN 					GPIO_Pin_5


#define GPIO_HIGH(a, b) 			a->BSRRL = b
#define GPIO_LOW(a, b)				a->BSRRH = b
#define GPIO_TOGGLE(a, b) 			a->ODR ^= b

uint8_t GetDHT22(uint8_t *payload);
void DHT22_Test(void);

#endif
