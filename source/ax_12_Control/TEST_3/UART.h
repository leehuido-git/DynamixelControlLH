#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

typedef unsigned char byte;

void UART0_init(long baud);
void UART0_putchar(char c);
void UART_putchar(char c, FILE *stream);
int UART0_getchar(char *c);
int UART0_gets(char *c);
void UART0_puts(char *c);

void uart_ax_init(unsigned int ubrr);
void uart_transmit(unsigned char data);

void All_Control_TEST(	unsigned int ID_number_1, int chose_1, unsigned int flexible_val_1,unsigned int flexible_1, unsigned int p_number_1, unsigned int AX_Speed_1,
unsigned int ID_number_2, int chose_2, unsigned int flexible_val_2,unsigned int flexible_2, unsigned int p_number_2, unsigned int AX_Speed_2,
unsigned int ID_number_3, int chose_3, unsigned int flexible_val_3,unsigned int flexible_3, unsigned int p_number_3, unsigned int AX_Speed_3,
unsigned int ID_number_4, int chose_4, unsigned int flexible_val_4,unsigned int flexible_4, unsigned int p_number_4, unsigned int AX_Speed_4,
unsigned int ID_number_5, int chose_5, unsigned int flexible_val_5,unsigned int flexible_5, unsigned int p_number_5, unsigned int AX_Speed_5,
unsigned int ID_number_6, int chose_6, unsigned int flexible_val_6,unsigned int flexible_6, unsigned int p_number_6, unsigned int AX_Speed_6,
unsigned int ID_number_7, int chose_7, unsigned int flexible_val_7,unsigned int flexible_7, unsigned int p_number_7, unsigned int AX_Speed_7,
unsigned int ID_number_8, int chose_8, unsigned int flexible_val_8,unsigned int flexible_8, unsigned int p_number_8, unsigned int AX_Speed_8,
unsigned int ID_number_9, int chose_9, unsigned int flexible_val_9,unsigned int flexible_9, unsigned int p_number_9, unsigned int AX_Speed_9,
unsigned int ID_number_10, int chose_10, unsigned int flexible_val_10,unsigned int flexible_10, unsigned int p_number_10, unsigned int AX_Speed_10);


void forward(void);
void backward(void);

void turnleft(void);
void turnright(void);

void sideleft(void);
void sideright(void);

void sitdown(void);
void standup(void);

void stand(void);
void ready(void);