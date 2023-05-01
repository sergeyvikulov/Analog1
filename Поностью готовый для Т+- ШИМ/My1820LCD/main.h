#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "lcd.h"
#include "DS18B20.h"
#include "usart.h"

#include "PWM.h"
void init_PWM_timer(void);

#endif /* MAIN_H_ */