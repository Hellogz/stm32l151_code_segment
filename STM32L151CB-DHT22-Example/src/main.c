/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 0.1.0
   Last changed by:    $Author: Hellogz
   Last changed date:  $Date:  2016/8/12
   ID:                 $Id:  Winext

**********************************************************************/
#include "stm32l1xx_conf.h"
#include "usart.h"
#include "dht22.h"
#include <stdio.h>

/*
NOTE: the Em:Blocks i don't know how to use printf output to USART.
*/

int main(void)
{
    SysTick_conf();

    debug_printf_com_config();

    PrintSystemClockSource();

    printf("MCU is work:%ld\r\n", SystemCoreClock);

    DHT22_Test();
}
