#ifndef __DS18B20_H__
#define __DS18B20_H__

#include"stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"

#define  SkipROM                0xCC    //跳过ROM
#define  SearchROM              0xF0    //搜索ROM
#define  ReadROM                0x33    //读ROM
#define  MatchROM               0x55    //匹配ROM
#define  AlarmROM               0xEC    //告警ROM

#define  StartConvert           0x44    //开始温度转换，在温度转换期间总线上输出0，转换结束后输出1
#define  ReadScratchpad         0xBE    //读暂存器的9个字节
#define  WriteScratchpad        0x4E    //写暂存器的温度告警TH和TL
#define  CopyScratchpad         0x48    //将暂存器的温度告警复制到EEPROM，在复制期间总线上输出0，复制完后输出1
#define  RecallEEPROM           0xB8    //将EEPROM的温度告警复制到暂存器中，复制期间输出0，复制完成后输出1
#define  ReadPower              0xB4    //读电源的供电方式：0为寄生电源供电；1为外部电源供电

#define EnableINT()
#define DisableINT()

#define DS_PORT   							GPIOD           //DS18B20 Port
#define DS_DQIO   							GPIO_Pin_2      //GPIOD6

#define DS_RCC_PORT  						RCC_AHBPeriph_GPIOD

#define DS_PRECISION 						0x7f   					//精度配置寄存器 1f=9位; 3f=10位; 5f=11位; 7f=12位;
#define DS_AlarmTH  						0x64
#define DS_AlarmTL  						0x8a
#define DS_CONVERT_TICK 				    1000

#define ResetDQ() 							GPIO_ResetBits(DS_PORT, DS_DQIO)
#define SetDQ()  							GPIO_SetBits(DS_PORT, DS_DQIO)
#define GetDQ()  							GPIO_ReadInputDataBit(DS_PORT, DS_DQIO)

void ds18b20_init(void);
float ds18b20_read(void);;

#endif

