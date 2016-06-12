/**
  ******************************************************************************
  * @file    app.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-January-2014
  * @brief   This file provides all the Application firmware functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/ 
#include "usbd_dfu_core.h"
#include "usbd_usr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USB_CORE_HANDLE  USB_Device_dev ;
pFunction Jump_To_Application;
uint32_t JumpAddress;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Program entry point
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f072.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
      */  

    /* Configure "DFU enter" button */
  STM_EVAL_PBInit(BUTTON_TAMPER, Mode_GPIO);
  
  /* Check if the TAMPER push-button on STM32-EVAL Board is pressed */
  if (STM_EVAL_PBGetState(BUTTON_TAMPER) != 0x00)
  { /* Test if user code is programmed starting from address 0x8003000 */
    if (((*(__IO uint32_t*)APP_DEFAULT_ADD) & 0x2FFE0000 ) == 0x20000000)
    { /* Jump to user application */
      
      JumpAddress = *(__IO uint32_t*) (APP_DEFAULT_ADD + 4);
      Jump_To_Application = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) APP_DEFAULT_ADD);
      Jump_To_Application();
    }
  } /* Otherwise enters DFU mode to allow user to program his application */
  
  
  /* The Application layer has only to call USBD_Init to 
  initialize the USB low level driver, the USB device library, the USB clock 
  ,pins and interrupt service routine (BSP) to start the Library*/
  
  USBD_Init(&USB_Device_dev,
            &USR_desc, 
            &DFU_cb, 
            &USR_cb);
  
  /* Setup SysTick Timer for 10 msec interrupts 
  This interrupt is used to display the current state of the DFU core */
  if (SysTick_Config(SystemCoreClock / 100))
  { 
    /* Capture error */ 
    while (1);
  }
 
  while (1)
  {
  }
} 

#ifdef USE_FULL_ASSERT
/**
  * @brief  assert_failed
  *         Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  File: pointer to the source file name
  * @param  Line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
