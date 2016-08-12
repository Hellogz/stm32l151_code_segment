/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32l1xx_conf.h"
#include "stm32l1xx.h"
#include "lcd_gpio_init.h"
#include "stm32l_discovery_lcd.h"
#include "stdio.h"

uint16_t strDisp[6]; //������ �������� ��� ������ �� �������
static volatile float t;

void ds18b20_init(void);
float ds18b20_read(void);
void convert_into_char(uint32_t number, uint16_t *p_tab);

int main(void)
{
    gpio_init();  //��������� ������ �����/������ ��� ������ � LCD
    lcd_init();   //����� ������������� LCD ����������
    ds18b20_init();

  while(1)
  {

    t = ds18b20_read();

    int t_int = t; //����� ����� ����� �����
    int t_float = ((t*10)-(t_int*10)); //����� ����� ������� �����
    int t_str = (t_int*10) + t_float;
    t_str = t_str*100;

    /*������������ ��������� ��������������  � ������ ��������.
    ��������� ����������� � ��������� ������� 1..4*/
    convert_into_char(t_str, strDisp);
    strDisp[3] = ' '; //������ ���������� ������� ��������� ������
    strDisp[4] = '�'; //����� "������"
    strDisp[5] = 'C'; //� ��������� ���������� ������� ������� ����� C
    strDisp[1] |= DOT; //���������� �����

    /*��������� ������� ������� �� ����� ������ strDisp. ��� ��� ��������� �������� ��
    ����� ���������� ����� ��� ���������  � ����� ������� ������.
    � ������ ������ ������� strDisp[1] |= DOT ������� ����� ����� ������ �����*/
    LCD_GLASS_DisplayStrDeci(strDisp);

  }
}

//**************************************************************************
/*
void delay(unsigned long p)
{
	while(p>0){p--;}
}
*/
//����� �������������� ��������� ��������� ������� assert_param() �� ����������� ���������
#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
  while (1)
  {
  }
}
#endif
//***********************************************************************
//��� ������� ������������ ��������� �������������� � ������ ��������
void convert_into_char(uint32_t number, uint16_t *p_tab)
{
  uint16_t units=0, tens=0, hundreds=0, thousands=0, misc=0;

  units = (((number%10000)%1000)%100)%10;
  tens = ((((number-units)/10)%1000)%100)%10;
  hundreds = (((number-tens-units)/100))%100%10;
  thousands = ((number-hundreds-tens-units)/1000)%10;
  misc = ((number-thousands-hundreds-tens-units)/10000);

  *(p_tab+4) = units + 0x30;
  *(p_tab+3) = tens + 0x30;
  *(p_tab+2) = hundreds + 0x30;
  *(p_tab+1) = thousands + 0x30;
  *(p_tab) = misc + 0x30;
}
