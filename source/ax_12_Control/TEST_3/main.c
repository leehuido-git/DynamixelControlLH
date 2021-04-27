/*
 * main.c
 *
 * Created: 2017-09-30
 * Author : leehuido
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>	//AVR Input output을 사용하기 위한 헤더파일
#include <util/delay.h>	//delay를 사용하기 위한 헤더파일
#include "UART.h"
#include "Timer.h"

void main(void)
{
	char *data;
	UART0_init(9600);	//보레이트 즉 통신속도를 설정 (통신속도에 따라 연산속도차이가 발생한다.)
	uart_ax_init(1);
	Timer0_init();
	sei();		// sei()함수는 인터럽트 발생을 전역적으로 허용
	stand();	//직립
	_delay_ms(1000);


    while(1)
    {
			if (UART0_gets(data) == 0)
			{
			}
			else
			{
				UART0_puts(data);	//값 확인용
			}

		if(*data == 't')
		{
			stand();	//직립
		}

		if(*data == 'z')
		{
			ready();	//준비 상태
		}

		if(*data == 'w')
		{
			forward();	//직립 전방 보행
		}

		if(*data == 's')
		{
			backward(); //직립 후방 보행
		}

		if(*data == 'a')
		{
			turnleft();	//좌 방향 전환
		}

		if(*data == 'd')
		{
			turnright();//우 방향 전환
		}

		if(*data == 'q')
		{
			sideleft();//좌 사이드 스텝
		}

		if(*data == 'e')
		{
			sideright();//우 사이드 스텝
		}

		if(*data == 'j')
		{
			sitdown();	//앉기
		}

		if(*data == 'u')
		{
			standup();	//앉은 상태에서 일어나기
		}
	}
}