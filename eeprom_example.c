#include "stm32l1xx_flash.h"

/*
EEPROM byte 读写例程。
*/

#define DATA_EEPROM_FLAG_ADDR      0x08080000
#define SET_CONFIGURE_FLAG         (uint32_t)0xAB
#define DATA_EEPROM_START_ADDR     0x08080004
#define DATA_EEPROM_END_ADDR       0x080803FF

void memcpy1( uint8_t *dst, const uint8_t *src, uint16_t size )
{
    while( size-- )
    {
        *dst++ = *src++;
    }
}

void memcpyr( uint8_t *dst, const uint8_t *src, uint16_t size )
{
    dst = dst + ( size - 1 );
    while( size-- )
    {
        *dst-- = *src++;
    }
}

void memset1( uint8_t *dst, uint8_t value, uint16_t size )
{
    while( size-- )
    {
        *dst++ = value;
    }
}

uint32_t byte_to_word(uint8_t *data, uint8_t size)
{
	uint32_t result = 0;
	
	if(size == 4)
	{
		memcpyr((uint8_t *)&result, data, size);
	}
	else
	{
		memcpyr((uint8_t *)&result, data, size);
		memset1((uint8_t *)&result + size, 0, 4-size);
	}
	return result;
}

uint8_t write_byte_buffer(uint8_t *data, uint8_t size)
{
	typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
	uint32_t NbrOfPage = 0, j = 0, Address = 0, end_address = 0;
	uint8_t index = 0;
	__IO FLASH_Status FLASHStatus = FLASH_COMPLETE;
	__IO TestStatus DataMemoryProgramStatus = PASSED;
	
	/* Unlock the FLASH PECR register and Data EEPROM memory */
	DATA_EEPROM_Unlock();

	/* Clear all pending flags */      
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
				  | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR | FLASH_FLAG_OPTVERRUSR);
	
	/*  Data EEPROM Fast Word program of FAST_DATA_32 at addresses defined by 
		DATA_EEPROM_START_ADDR and DATA_EEPROM_END_ADDR */  
	Address = DATA_EEPROM_START_ADDR;

	NbrOfPage = ((DATA_EEPROM_END_ADDR - Address) + 1 ) >> 2; 

	/* Erase the Data EEPROM Memory pages by Word (32-bit) */
	for(j = 0; j < NbrOfPage; j++)
	{
		FLASHStatus = DATA_EEPROM_EraseWord(Address + (4 * j));
	}

	while(Address < DATA_EEPROM_END_ADDR)
	{
		if(*(__IO uint32_t*)Address != 0x0)
		{
			DBG("*(__IO uint32_t*)Address:0x%08X != 0x0\r\n", Address);
			DataMemoryProgramStatus = FAILED;
		}
		Address = Address + 4;
	}
	
	Address = DATA_EEPROM_START_ADDR;
	end_address = Address + (uint32_t)size;
	
	while(Address <= end_address )
	{
		FLASHStatus = DATA_EEPROM_ProgramByte(Address, data[index]);

		if(FLASHStatus == FLASH_COMPLETE)
		{
			Address = Address + 1;
			index = index + 1;
		}
		else
		{
			FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
				| FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR);
		}
	}

	Address = DATA_EEPROM_START_ADDR;
	index = 0;

	/* Check the correctness of written data */
	while(Address < end_address)
	{
		if(*(__IO uint8_t*)Address != data[index])
		{
			DBG("*(__IO uint32_t*)Address:0x%08X != Data:%08X\r\n", Address, data[index]);
			DataMemoryProgramStatus = FAILED;
		}
		Address = Address + 1;
		index = index + 1;
	}
	
	DATA_EEPROM_ProgramWord(DATA_EEPROM_FLAG_ADDR, SET_CONFIGURE_FLAG);
	if(*(__IO uint32_t*)DATA_EEPROM_FLAG_ADDR != SET_CONFIGURE_FLAG)
	{
		DBG("Write set connfigure filed\r\n");
		DataMemoryProgramStatus = FAILED;
	}
	
	if (DataMemoryProgramStatus == FAILED)
	{
		DBG("Write Fialed\r\n");
	}
	else
	{
		DBG("Write Success\r\n");
	}
	
	DATA_EEPROM_Lock();
	
	return DataMemoryProgramStatus;
}

void get_eeprom_data(uint8_t *buffer, uint8_t size)
{
	uint32_t Address = 0, end_address = 0;
	uint8_t index = 0;
	
	Address = DATA_EEPROM_START_ADDR;
	end_address = Address + (uint32_t)size;
	
	while(Address < end_address)
	{
		buffer[index] = *(__IO uint8_t*)Address;

// 			DBG("*(__IO uint32_t*)Address:0x%08X = Data:0x%02X->%d\r\n", Address, buffer[index], buffer[index]);

		Address = Address + 1;
		index = index + 1;
	}
}