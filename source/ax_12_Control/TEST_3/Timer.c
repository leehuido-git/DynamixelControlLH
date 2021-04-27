#define F_CPU 16000000UL

#include "Timer.h"
#include <util/delay.h>


int time= 0;

int Timer0_1ms_cnt =0;
int Timer0_1ms_flag =0;

ISR(TIMER0_OVF_vect)	// 어떤 인터럽트가 발생했을 때 이를 받아서 처리하는 함수를 ISR함수라고 한다. TIMER1_OVF_vect: 타이머/카운터1 오버플로
{
	if(Timer0_1ms_cnt++ ==1)
	{
		Timer0_1ms_cnt =0;
		Timer0_1ms_flag =1;
	}
	TCNT0 =6;
}


void Timer0_init()
{
	TCCR0 = (0 << WGM01) | (0 << WGM00) | (0 << COM01) | (0 << COM00) | (1 << CS02) | (0 << CS01) | (0 << CS00);
	// Normal mode, OC pin - Normal I/O, 분주율 64
	
	TIMSK = (0 << OCIE0) | (1 << TOIE0);
	// Compare-False, Overflow-True
}


int Timer0_flag(int ms)
{
	if(ms ==1)
	{
		if(Timer0_1ms_flag ==1)
		{
			Timer0_1ms_flag =0;
			return 1;
		}
	}
	return 0;
}