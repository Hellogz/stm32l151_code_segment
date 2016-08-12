#include "stm32l1xx.h"
#include "tick_delay.h"
#include "usart.h"

#define SYSTICK     0  // use timer:0, use systick:1

static __IO uint32_t TimingDelay;

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void TickDelay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}

/*!
 * Timer IRQ handler
 */
void TIM6_IRQHandler( void )
{
    if( TIM_GetITStatus( TIM6, TIM_IT_Update ) != RESET )
    {
        if (TimingDelay != 0x00)
		{
			TimingDelay--;
		}
        TIM_ClearITPendingBit( TIM6, TIM_IT_Update );
    }
}

void SysTick_conf(void)
{
#if SYSTICK
    if (SysTick_Config(SystemCoreClock / 2000)) // 0.5 ms
    {
        /* Capture error */
        printf("Capture error\r\n");
        while (1);
    }
#else
    /* TIM 6 Init */
	NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* TIM2 clock enable */
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM6, ENABLE );

    /* --------------------------NVIC Configuration -------------------------------*/
    /* Enable the TIM2 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init( &NVIC_InitStructure );

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 1599;    //  32MHz clock :0.5 ms
    TIM_TimeBaseStructure.TIM_Prescaler = 10;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM6, &TIM_TimeBaseStructure );

    TIM_ITConfig( TIM6, TIM_IT_Update, ENABLE );

    /* TIM2 disable counter */
    TIM_Cmd( TIM6, ENABLE );
#endif
}

void PrintSystemClockSource(void)
{
 /*
  *              - 0x00: MSI used as system clock
  *              - 0x04: HSI used as system clock
  *              - 0x08: HSE used as system clock
  *              - 0x0C: PLL used as system clock
  */
	switch(RCC_GetSYSCLKSource())
	{
	case 0x00: printf("MSI used as system clock\r\n"); break;
	case 0x04: printf("HSI used as system clock\r\n"); break;
	case 0x08: printf("HSE used as system clock\r\n"); break;
	case 0x0C: printf("PLL used as system clock\r\n"); break;
	}
}


