#include "PWM.h"

void init_PWM_timer(void)
{
	ASSR=0x00;
	TCCR2=0b01101110; //Fast PWM, Clear OC2 on Compare Match, clkT2S/256 (From prescaler) (реальная частота получится 8мгц/256 = 31250 гц)
	TCNT2=0x00; // Timer Value = 0
	OCR2=0x00; //Output Compare Register = 0 - в нашем случае это скважность шим (0 - это 0, а FF - 100 %)
	TIMSK|=0x00;//
}
