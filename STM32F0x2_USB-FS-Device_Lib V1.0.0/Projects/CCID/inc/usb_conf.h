/**
  ******************************************************************************
  * @file    usb_conf.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-January-2014
  * @brief   General low level driver configuration
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CONF__H__
#define __USB_CONF__H__

/* Includes ------------------------------------------------------------------*/

#include "stm32f0xx.h"
#include "stm32072B_eval.h"



/** @addtogroup USB_DEVICE_DRIVER
  * @{
  */
  
/** @defgroup USB_CONF
  * @brief USB low level driver configuration file
  * @{
  */ 

/** @defgroup USB_CONF_Exported_Defines
  * @{
  */ 

/* Select D+ pullup: internal or external */
#ifdef USE_STM32072B_EVAL
 /* When using STM32072B_EVAL board the internal pullup must be enabled */
 #define INTERNAL_PULLUP
#endif

/* Define if Low power mode is enabled; it allows entering the device into STOP mode
    following USB Suspend event, and wakes up after the USB wakeup event is received. */
/* #define USB_DEVICE_LOW_PWR_MGMT_SUPPORT */

/* Selects the USB clock source as HSI48 (otherwise PLL clock will be selected, should be 48MHz!)*/
#define USB_CLOCK_SOURCE_CRS


/* Endpoints used by the device */
#define EP_NUM    (3) /* EP0 + EP1 for CCID IN/OUT + EP2 for CCID INT */

/* Buffer table base address */
#define BTABLE_ADDRESS    (0x00)


/* EP0, RX/TX buffers base address */
#define ENDP0_RX_ADDRESS    (0x40)
#define ENDP0_TX_ADDRESS    (0x80)

/* EP1/EP2, TX buffer base address */
#define CCID_BULK_TX_ADDRESS  (0xC0)  
#define CCID_INT_TX_ADDRESS   (0x100)

/* EP1 RX buffer base address */
#define CCID_BULK_RX_ADDRESS  (0x110)

/**
  * @}
  */ 


/** @defgroup USB_CONF_Exported_Types
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USB_CONF_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_CONF_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_CONF_Exported_FunctionsPrototype
  * @{
  */ 
/**
  * @}
  */ 


#endif /* __USB_CONF__H__ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
