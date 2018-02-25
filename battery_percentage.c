// battery_percentage.c Start

#include "battery_percentage.h"

static const float Set_Voltage[] =
{
	4.20, 4.06, 3.98, 3.92, 3.87, 3.82, 3.79, 3.77, 3.74, 3.68, 3.45, 3.00
};

static const uint8_t Set_Percentage[] =
{
	100, 90, 80, 70, 60, 50, 40, 30, 20, 10, 5, 0
};

static float Get_Battery_Adc(void)
{
  uint16_t Battle_temp = 0;
  float Battle_ADC = 0.0;
  static float old_bat = 0;
	
	Gpio_t Gpio;
	Adc_t Adc;
	// Get Battery ADC Value.
	AdcInit( &Adc, BATTLE_CHECK );

  Battle_temp = AdcReadChannel( &Adc, BATTLE_CHECK_CHANNEL );
 //                                vDiv
 // Divider bridge  VBAT <-> 1M -<--|-->- 1M <-> GND => vBat = 2 * vDiv
  Battle_ADC = (float)(Battle_temp) * (3.3 / 4095) * 2;  

  PRINTF("Battery:%fV, ", Battle_ADC);

	GpioInit( &Gpio, DC_IN_PIN, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );

	if(old_bat == 0)	// system up
	{
		old_bat = Battle_ADC;
	}
	
	if(GpioRead(&Gpio))
	{
		PRINTF("Now is DC IN, ");

		if(Battle_ADC > old_bat)
		{
			old_bat = Battle_ADC;
		}	
		else
		{
			Battle_ADC = old_bat;
		}
	}
	else
	{		
		if(Battle_ADC > old_bat)
		{
			Battle_ADC = old_bat;			
		}	
		else
		{
			old_bat = Battle_ADC;
		}
	}
	
	GpioInit( &Gpio, DC_IN_PIN, PIN_ANALOGIC, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
	
    PRINTF("Battery = %fV, Old = %fV\r\n", Battle_ADC, old_bat);
	
	return Battle_ADC;
}

static uint8_t cal_percentage(float adc_value)
{
	uint8_t length = sizeof(Set_Voltage);
	uint8_t i  = 0;
	
	for(i = 0; i < length ; ++i) {
		if(adc_value >= Set_Voltage[i]) {
			break;
		}
	}
	
    if( i >= length) {
        return 0;
    }

    if(i == 0) {
        return 100;
    }

    return (adc_value - Set_Voltage[i])/((Set_Voltage[i-1] - Set_Voltage[i])/(Set_Percentage[i-1] - Set_Percentage[i])) + Set_Percentage[i];
}

uint8_t get_battery_percentage(void)
{
	float adc_vale = 0;
	uint8_t battery_percentage = 0;
	
	adc_vale = Get_Battery_Adc();
	battery_percentage = cal_percentage(adc_vale);
	
	return battery_percentage;
}

void test_get_battery_percentage(void)
{
	while(1)
	{
		PRINTF("Battery: %d%%\r\n", get_battery_percentage());
		DelayMs(2000);
	}
}
// battery_percentage.c End

// battery_percentage.h Start

#ifndef BATTERY_PERCENTAGE_H

extern uint8_t get_battery_percentage(void);
extern void test_get_battery_percentage(void);

#endif

// battery_percentage.c End


