#ifndef DS18B20_H_
#define DS18B20_H_

#include "main.h"

#define NOID 0xCC //Пропустить идентификацию
#define T_CONVERT  0x44 //Код измерения температуры
#define READ_DATA 0xBE //Передача байтов ведущему



#define PORTTEMP PORTB		
#define DDRTEMP DDRB	
#define PINTEMP PINB	
#define BITTEMP 0

int dt_check(void); //функция преобразования показаний датчика в температуру
char converttemp (unsigned int tto); //преобразование температуры в единицы
char converttempm (unsigned int ttm);
char read_znak(unsigned int ttz); //узнаем положительная или отрицательная температура
char converttempczm(unsigned int czm);
char converttempdrzm(unsigned int drzm);

#endif /* DS18B20_H_ */