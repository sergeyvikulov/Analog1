#include "main.h"
//----------------------------------------
//unsigned char sec,min,hour,day,date,month,year;
//----------------------------------------
void port_ini(void)
{
	PORTD=0x00;
	DDRD=0xFF;
	PORTB=0x00;
	DDRB=0x08;
}
//----------------------------------------
int main(void)
{
//	unsigned char pwm_state=0;	//!!!
	
	unsigned int to=0;		//переменная для хранения температуры
	unsigned int tm=0;		//переменная для хранения дробной части температуры
	unsigned int znak=0;	//какой знак + или -
	unsigned int czm=0;		//переменая целого значения температуры при отриц
	unsigned int drzm=0;	//переменая дроби температуры при отриц
	unsigned int obchee=0;	//переменная всего числа
	unsigned int tmin=0;		//переменная которой присваивается общее и знак меняется на противоположный
	
	port_ini(); //Инициализируем порты
	
	
	//LCD_ini();  //Инициализируем дисплей
	//clearlcd(); //Очистим дисплей
	//USART_Init (103); //9600
	init_PWM_timer();			//!!!
	OCR2 = 0;					//!!!
	tmin = 0;

	while(1)
	{	
		clearlcd();
		znak = read_znak(dt_check()); //флаг знака. Если 0 отриц, если 1 полож
		tmin=0;
		if (znak==1) 
		{	
			to = converttemp(dt_check());//измеряем температуру без долей
			tm = converttempm(dt_check());//измеряем температуру доли градуса
			obchee = (to*10+tm%10)/10;
			OCR2=51+(obchee+40)*2.038;
				if (obchee>=60)
				{
					OCR2=20;
				}
				else
				{
					OCR2=51+(obchee+40)*2.038;
					if (OCR2>=254) OCR2=20;
				}
		}
		else		
		{
			czm = converttempczm(dt_check());//измеряем температуру без долей при отриц
			drzm = converttempdrzm(dt_check());//измеряем температуру доли градуса
 			obchee = (czm*10+drzm%10)/10;
			tmin=-obchee; 
				if (obchee>40)
				{
					OCR2=20;
				}
				else
				{
					OCR2=512/10+(tmin+40)*2.038;
					if (OCR2>=254) OCR2=20;
				}
}
		_delay_ms(1000);

	}
}


