#include "DS18B20.h"

//функция определения датчика на шине
char dt_testdevice(void) //dt - digital termomether | определим, есть ли устройство на шине
{
	char stektemp=SREG;// сохраним значение стека
	cli(); //запрещаем прерывание
	char dt;
	DDRTEMP |= 1<<BITTEMP; //притягиваем шину
	_delay_us(485); //задержка как минимум на 480 микросекунд
	DDRTEMP &= ~(1<<BITTEMP); //отпускаем шину
	_delay_us(65); //задержка как максимум на 60 микросекунд
	if ((PINTEMP & (1<<BITTEMP))==0)//проверяем, ответит ли устройство
	{
		dt=1;//устройство есть
	} 
	else dt=0;//устройства нет
	SREG = stektemp;// вернем значение стека
	_delay_us(420); //задержка как минимум на 480 микросекунд, но хватит и 420, тк это с учетом времени прошедших команд
	return dt; //вернем результат
}


//функция записи бита на устройство
void dt_sendbit(char bt)
{
	char stektemp=SREG;// сохраним значение стека
	cli(); //запрещаем прерывание
	DDRTEMP |= 1<<BITTEMP; //притягиваем шину
	_delay_us(2); //задержка как минимум на 2 микросекунды
	if(bt)
		DDRTEMP &= ~(1<<BITTEMP); //отпускаем шину
	_delay_us(65); //задержка как минимум на 60 микросекунд
	DDRTEMP &= ~(1<<BITTEMP); //отпускаем шину
	SREG = stektemp;// вернем значение стека
}
//функция записи байта на устройство
void dt_sendbyte(unsigned char bt)
{
	char i;		
	for(i=0;i<8;i++)//посылаем отдельно каждый бит на устройство
	{
		if((bt & (1<<i)) == 1<<i)//посылаем 1
			dt_sendbit(1);
		else //посылаем 0
			dt_sendbit(0);
	}	
}

//функция чтения бита с устройства
char dt_readbit(void)
{
	char stektemp=SREG;// сохраним значение стека
	cli(); //запрещаем прерывание
	char bt; //переменная хранения бита
	DDRTEMP |= 1<<BITTEMP; //притягиваем шину
	_delay_us(2); //задержка как минимум на 2 микросекунды
	DDRTEMP &= ~(1<<BITTEMP); //отпускаем шину
	_delay_us(13);
	bt = (PINTEMP & (1<<BITTEMP))>>BITTEMP; //читаем бит
	_delay_us(45);
	SREG = stektemp;// вернем значение стека
	return bt; //вернем результат
}

//функция чтения байта с устройства
unsigned char dt_readbyte(void)
{
	char c=0;
	char i;
	for(i=0;i<8;i++)
		c|=dt_readbit()<<i; //читаем бит
	return c;
}

//функция преобразования показаний датчика в температуру
int dt_check(void)
{
	unsigned char bt;//переменная для считывания байта
	unsigned int tto=0;
	unsigned int ttm=0;
	unsigned int ttczm=0;
	unsigned int ttdrzm=0;
	unsigned int ttz=0;

	if(dt_testdevice()==1) //если устройство нашлось
	{
		dt_sendbyte(NOID); //пропустить идентификацию, тк у нас только одно устройство на шине
		dt_sendbyte(T_CONVERT); //измеряем температуру
		_delay_ms(1000); //в 12битном режиме преобразования - 750 милисекунд
		dt_testdevice(); //снова используем  те же манипуляции с шиной что и при проверке ее присутствия
		dt_sendbyte(NOID); //пропустить идентификацию, тк у нас только одно устройство на шине
		dt_sendbyte(READ_DATA); //даем команду на чтение данных с устройства
		bt = dt_readbyte(); //читаем младший бит
		tto = dt_readbyte(); //читаем старший бит MS
		tto = (tto<<8)|bt;//сдвигаем старший влево, младший пишем на его место, тем самым получаем общий результат
		ttm=tto;
		ttczm=tto;
		ttdrzm=tto;
		ttz=tto;
	}
	return tto;
	return ttm;
	return ttczm;
	return ttdrzm;
	return ttz;
	
}

// Функция где узнаем положительная или отрицательная температура
char read_znak(unsigned int ttz)
{
	int tz=ttz;
	int znak=0;
	//char gz = 1;
	tz = (tz>>15);
	tz &=~0111111111111111;
	if (tz==0b0000000000000000)
	//tz &=~0000011111111111;
	//if (tz<0xf800)		// если последние 5 бит старшего байта = 11111
		(znak=1);	//минусовая
	else 
		(znak = 0);	//плюсовая
	return znak;
}

//преобразование температуры в единицы
char converttemp (unsigned int tto)
{
	char to = tto>>4;//сдвиг и отсечение части старшего байта
	return to;
		
}
char converttempm (unsigned int ttm)
{ttm &=~1111111111110000; //выделяем младший байт (выставляем все нули кроме дробной части - первые 4 бита это дробь) 
	char tm = ttm;
return tm; // tm - это и есть дробная часть
}

char converttempczm(unsigned int ttczm)
{
	int zm=ttczm;
	int czm;
	zm=~zm;
	zm++;
	czm = zm;
	czm = czm>>4;
	czm &=~1111111100000000;
	return czm;
}
char converttempdrzm(unsigned int ttdrzm)
{
	int zm=ttdrzm;
	int drzm;
	zm=~zm;
	zm++;
	drzm = zm;
	drzm &=~1111111111110000;
	return drzm;
}