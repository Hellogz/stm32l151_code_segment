#include "ds18b20.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"

void Delay_us(uint32_t Nus)
{
  SysTick->LOAD=Nus*9;
  SysTick->CTRL|=0x01;
  while(!(SysTick->CTRL&(1<<16)));
  SysTick->CTRL=0X00000000;
  SysTick->VAL=0X00000000;
}

void ResetDS18B20(void)
{
    uint32_t count = 0;
    SetDQ();
    Delay_us(50);

    ResetDQ();
    Delay_us(500);  //500us （该时间的时间范围可以从480到960微秒）
    SetDQ();
    Delay_us(40);  //40us
    while(GetDQ() && count++ < 0xFFFF); // 防止未连接传感器时一直无法退出while
    Delay_us(500);  //500us
    SetDQ();
}

void DS18B20WriteByte(uint8_t Dat)
{
    uint8_t i;
    for(i = 8; i > 0; i--)
    {
        ResetDQ();     //在15u内送数到数据线上，DS18B20在15-60u读数
        Delay_us(5);    //5us
        if(Dat & 0x01) { SetDQ(); }
        else { ResetDQ(); }
        Delay_us(65);    //65us
        SetDQ();
        Delay_us(2);    //连续两位间应大于1us
        Dat >>= 1;
    }
}


uint8_t DS18B20ReadByte(void)
{
    uint8_t i, Dat;
    SetDQ();
    Delay_us(5);
    for(i = 8; i > 0; i--)
    {
        Dat >>= 1;
        ResetDQ();     //从读时序开始到采样信号线必须在15u内，且采样尽量安排在15u的最后
        Delay_us(5);   //5us
        SetDQ();
        Delay_us(5);   //5us
        if(GetDQ()){ Dat |= 0x80; }
        else { Dat &= 0x7f; }
        Delay_us(65);   //65us
        SetDQ();
    }
    return Dat;
}


void ReadRom(uint8_t *Read_Addr)
{
    uint8_t i;

    DS18B20WriteByte(ReadROM);

    for(i = 8; i > 0; i--)
    {
        *Read_Addr=DS18B20ReadByte();
        Read_Addr++;
    }
}


void DS18B20Init(uint8_t Precision, uint8_t AlarmTH, uint8_t AlarmTL)
{
    uint32_t count = 0;
    DisableINT();
    ResetDS18B20();
    DS18B20WriteByte(SkipROM);
    DS18B20WriteByte(WriteScratchpad);
    DS18B20WriteByte(AlarmTL);
    DS18B20WriteByte(AlarmTH);
    DS18B20WriteByte(Precision);

    ResetDS18B20();
    DS18B20WriteByte(SkipROM);
    DS18B20WriteByte(CopyScratchpad);
    EnableINT();

    while(!GetDQ()) { if(count++ >0xFFFF) return; } //等待复制完成
}


void DS18B20StartConvert(void)
{
    DisableINT();
    ResetDS18B20();
    DS18B20WriteByte(SkipROM);
    DS18B20WriteByte(StartConvert);
    EnableINT();
}

void DS18B20_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(DS_RCC_PORT, ENABLE);

    //================ GPIOD CONFIGURATION ================
    GPIO_InitStructure.GPIO_Pin = DS_DQIO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(DS_PORT, &GPIO_InitStructure);
}


void ds18b20_init(void)
{
    DS18B20_Configuration();
    DS18B20Init(DS_PRECISION, DS_AlarmTH, DS_AlarmTL);
    DS18B20StartConvert();
}


float ds18b20_read(void)
{
    uint8_t DL, DH;
    uint16_t TemperatureData;
    float Temperature;

    DisableINT();
    DS18B20StartConvert();
    ResetDS18B20();
    DS18B20WriteByte(SkipROM);
    DS18B20WriteByte(ReadScratchpad);
    DL = DS18B20ReadByte();
    DH = DS18B20ReadByte();
    EnableINT();

    TemperatureData = DH;
    TemperatureData <<= 8;
    TemperatureData |= DL;

    Temperature = (float)((float)TemperatureData * 0.0625); //分辨率为0.0625度

    return  Temperature;
}

uint8_t crc8(uint8_t *addr, uint8_t len)
{
    uint8_t crc = 0, i;

    while (len--) {
        uint8_t inbyte = *addr++;
        for (i = 8; i; i--) {
            uint8_t mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}

int16_t ds18b20_read_crc(void)
{
    uint8_t data[9] = {0}, crc = 0, i;
    uint16_t TemperatureData;
    float Temperature;

    DisableINT();
    DS18B20StartConvert();
    ResetDS18B20();
    DS18B20WriteByte(SkipROM);
    DS18B20WriteByte(ReadScratchpad);

		for(i = 0; i < 9; i++)
		{
				data[i] = DS18B20ReadByte();
		}
		crc = crc8(data, 8);
/*
		DBG("9Byte:");
		for(uint8_t i = 0; i < 9; i++)
		{
				DBG(" %02X", data[i]);
		}
		DBG(" CRC=%02X\r\n", crc);
*/
    EnableINT();

		if(crc != data[8])
		{
            //DBG("\r\n===============CRC Error:%d!=%d===============\r\n\r\n", data[8], crc);
            return 0xFFFF;
		}

    TemperatureData = data[1];//DH;
    TemperatureData <<= 8;
    TemperatureData |= data[0];//DL;

    Temperature = (float)((float)TemperatureData * 0.0625); //分辨率为0.0625度

    return  Temperature * 100;
}

