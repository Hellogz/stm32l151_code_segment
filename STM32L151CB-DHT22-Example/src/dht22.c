#include "dht22.h"

// the value must printf dt[42] value and look, set SENSOR_OUTPUT_HIGH_VALUE between the maximum and minimum values
// for example max: 100 min: 50 so the value set to 60.
#define SENSOR_OUTPUT_HIGH_VALUE	40	

uint16_t read_cycle(uint16_t cur_tics, uint8_t neg_tic){
	uint16_t cnt_tics;
 	if (cur_tics < MAX_TICS) cnt_tics = 0;
	if (neg_tic){
		while (!GPIO_ReadInputDataBit(DHT22_PORT, DHT22_PIN)&&(cnt_tics<MAX_TICS)){
			cnt_tics++;
		}
	}
	else {
		while (GPIO_ReadInputDataBit(DHT22_PORT, DHT22_PIN)&&(cnt_tics<MAX_TICS)){
			cnt_tics++;
		}
	}
 	return cnt_tics;
}

uint8_t DHT22_RawRead(uint8_t *buf){
	GPIO_InitTypeDef GPIO_InitStructure;
	uint16_t dt[42];
	uint16_t cnt;
	uint8_t i, check_sum;

	GPIO_InitStructure.GPIO_Pin = DHT22_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(DHT22_PORT, &GPIO_InitStructure);

	//reset DHT22
	TickDelay(500);
 	GPIO_LOW(DHT22_PORT, DHT22_PIN);
	TickDelay(20);
 	GPIO_HIGH(DHT22_PORT, DHT22_PIN);

	GPIO_InitStructure.GPIO_Pin = DHT22_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(DHT22_PORT, &GPIO_InitStructure);

	//start reading
 	cnt = 0;
	for(i=0; i<83 && cnt<MAX_TICS; i++){
		if (i & 1){
			cnt = read_cycle(cnt, 1);
		}
		else {
			cnt = read_cycle(cnt, 0);
			dt[i/2]= cnt;
		}
	}

 	//release line
	GPIO_HIGH(DHT22_PORT, DHT22_PIN);

	for (i = 0; i<5; i++) buf[i]=0;

	if (cnt>=MAX_TICS) return DHT22_NO_CONN;

	//convert data
 	for(i=2;i<42;i++){
		(*buf) <<= 1;
		if (dt[i]>SENSOR_OUTPUT_HIGH_VALUE) {
			(*buf)++;
 		}
		if (!((i-1)%8) && (i>2)) {
			buf++;
		}
 	}

	//calculate checksum
	buf -= 5;
	check_sum = 0;
 	for(i=0;i<4;i++){
		check_sum += *buf;
		buf++;
	}

	if (*buf != check_sum) return DHT22_CS_ERROR;

	return DHT22_OK;
	//return check_sum;
}

uint16_t DHT22_Humidity(uint8_t *buf){
	uint16_t res;
	res = buf[0] * 256 + buf[1];
	return res * 10;
}

int16_t DHT22_Temperature(uint8_t *buf){
	int16_t res;
	res = (buf[2] & 0x7F)* 256 + buf[3];
	if (buf[2] & 0x80) res *= -1;
	return res * 10;
}

uint8_t GetDHT22(uint8_t *payload)
{
    uint8_t dhtbuf[5] = {0}, result = 0, count = 2;
    uint16_t humi = 0;
    int16_t temp = 0;

    while(count--)
	{
		result = DHT22_RawRead(dhtbuf);
		if(result == 0)
		{
			humi = DHT22_Humidity(dhtbuf);
			temp = DHT22_Temperature(dhtbuf);
			break;
		}
		else
		{
			printf("Get Failed:%d\r\n", result);
			humi = 0xFFFF;
			temp = 0xFFFF;
		}
		TickDelay(1000);
	}

	payload[0] = (temp >> 8) & 0xFF;
	payload[1] = temp & 0xFF;
	payload[2] = (humi >> 8) & 0xFF;
	payload[3] = humi & 0xFF;
	return 4;
}

void DHT22_Test(void)
{
	uint8_t dhtbuf[5] = {0}, result;
	uint32_t count = 1;

	RCC_AHBPeriphClockCmd(DHT22_GPIO_CLOCK, ENABLE);
	while(1)
	{
//	    printf("[Test DHT22 Count:%08d] \r\n", count++);
		printf("[Test DHT22 Count:%08ld] ", count++);
		result = DHT22_RawRead(dhtbuf);
		if(result == 0)
		{
			//printf("Humi:%2.1f%%RH; Temp:%2.1fC\r\n", DHT22_Humidity(dhtbuf), DHT22_Temperature(dhtbuf));
			printf("Humi:%d%%RH; Temp:%dC\r\n", DHT22_Humidity(dhtbuf), DHT22_Temperature(dhtbuf));
		}
		else
		{
			printf("Get Failed:%d\r\n", result);
		}
		TickDelay(1000);
	}
}
