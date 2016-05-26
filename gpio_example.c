/*
GPIO digital output
*/
#include "stm32lxx.h"

#define GPIO_CLK                        RCC_AHBPeriph_GPIOA
#define GPIO_PIN                        GPIO_Pin_0
#define GPIO_PORT                       GPIOA
#define GPIO_MODE                       GPIO_Mode_OUT
#define GPIO_OTYPE                      GPIO_OType_PP
#define GPIO_SPEED                      GPIO_Speed_10MHz
#define GPIO_PUPD                       GPIO_PuPd_NOPULL

void gpio_config(void)
{
  GPIO_InitTypeDef        GPIO_InitStructure;

  /* GPIO Periph clock enable */
  RCC_AHBPeriphClockCmd(GPIO_CLK, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_MODE;
  GPIO_InitStructure.GPIO_OType = GPIO_OTYPE;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PUPD;
    
  GPIO_Init(GPIO_PORT, &GPIO_InitStructure);
}

/*
Common method

uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);

uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);

void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal); Bit_RESET or Bit_SET

void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);

void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
*/
