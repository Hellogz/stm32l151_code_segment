/*
GPIO PB9 Exti interrupt example for stm32f10x
*/

void exti9_5_config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
/******************** Config GPIO ******************************/
	/* Config GPIO */
	/* Enable GPIOB clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* Configure PB.09 pin as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
/******************** Config GPIO end *************************/
	/* Enable AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	/* Connect EXTI9 Line to PB.09 pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);

	/* Configure EXTI9 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI9_5 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		/* do something */
		// add code to this
		/* Clear the  EXTI line 9 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line9);
	}	
}