/*
 * main.c
 *
 * Created: 2017-09-30
 * Author : leehuido
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>	//AVR Input output�� ����ϱ� ���� �������
#include <util/delay.h>	//delay�� ����ϱ� ���� �������
#include "UART.h"
#include "Timer.h"

void main(void)
{
	char *data;
	UART0_init(9600);	//������Ʈ �� ��żӵ��� ���� (��żӵ��� ���� ����ӵ����̰� �߻��Ѵ�.)
	uart_ax_init(1);
	Timer0_init();
	sei();		// sei()�Լ��� ���ͷ�Ʈ �߻��� ���������� ���
	stand();	//����
	_delay_ms(1000);


    while(1)
    {
			if (UART0_gets(data) == 0)
			{
			}
			else
			{
				UART0_puts(data);	//�� Ȯ�ο�
			}

		if(*data == 't')
		{
			stand();	//����
		}

		if(*data == 'z')
		{
			ready();	//�غ� ����
		}

		if(*data == 'w')
		{
			forward();	//���� ���� ����
		}

		if(*data == 's')
		{
			backward(); //���� �Ĺ� ����
		}

		if(*data == 'a')
		{
			turnleft();	//�� ���� ��ȯ
		}

		if(*data == 'd')
		{
			turnright();//�� ���� ��ȯ
		}

		if(*data == 'q')
		{
			sideleft();//�� ���̵� ����
		}

		if(*data == 'e')
		{
			sideright();//�� ���̵� ����
		}

		if(*data == 'j')
		{
			sitdown();	//�ɱ�
		}

		if(*data == 'u')
		{
			standup();	//���� ���¿��� �Ͼ��
		}
	}
}