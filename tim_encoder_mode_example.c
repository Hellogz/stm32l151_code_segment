/*
基于定时器的编码器模式来采集编码器的脉冲值。

void tim3_encoder_configration(void) 为STM32L151 使用
void t5_encoder_configration(void) 为STM32F10X使用
*/

#include"stm32l1xx_tim.h"

void tim3_encoder_configration(void)
{ 
  GPIO_InitTypeDef            GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
  TIM_ICInitTypeDef           TIM_ICInitStructure;

  /* TIM3 clock source enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Connect PA6 to TIM3_CH_1 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);

  /* Connect PA7 to TIM3_CH_2 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;    // 使用 PA6,PA7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  /* Timer configuration in Encoder mode */
  TIM_DeInit(TIM3);                   //TIM3 复位

  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
  TIM_TimeBaseStructure.TIM_Period = 10000;  
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  // 设置TIM3为编码模式1, 编码器值x2
  TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
  TIM_ICStructInit(&TIM_ICInitStructure);    // 初始化
  TIM_ICInitStructure.TIM_ICFilter = 15;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);

  //Reset counter
  TIM3->CNT = 0;  // 该寄存器为编码器的值
  TIM_Cmd(TIM3, ENABLE); 
}

/************************************************************************************************/
 #include "stm32f10x_tim.h"


void t5_encoder_configration(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;

  TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;

 /* TIM5 clock source enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

//PA0-A相, PA1-B相                                                 
  GPIO_StructInit(&GPIO_InitStructure);                     
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;    // 使用PA0,PA1

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);


  /* Timer configuration in Encoder mode */
  TIM_DeInit(TIM5);                   //TIM5复位 
  
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
  TIM_TimeBaseStructure.TIM_Period = 10000;  
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
 
 // 设TIM5为编码模式3
  TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICStructInit(&TIM_ICInitStructure);    // 初始化为默认值
  TIM_ICInitStructure.TIM_ICFilter = 6;
  TIM_ICInit(TIM5, &TIM_ICInitStructure);
   
  //Reset counter
  TIM5->CNT = 0;
  TIM_Cmd(TIM5, ENABLE); 

}

//  TIM5->CNT的内容就是编码器的计数脉冲。
/************************************************************************************************/