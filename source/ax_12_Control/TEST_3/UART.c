#include "UART.h"
#include <util/delay.h>
#define ON 1
#define OFF 0

unsigned char uart0_rx_flag =0;
unsigned char uart0_rx_data;

static FILE mystdout = FDEV_SETUP_STREAM(UART_putchar, NULL, _FDEV_SETUP_WRITE);
// FDEV_SETUP_STREAM은 매크로 함수이다. 즉 매크로 함수를 통해서 위 소스 코드에서 정의한 UART_putchar함수와 printf를 연결해주고 있다.


ISR (USART0_RX_vect)		// 어떤 인터럽트가 발생했을 때 이를 받아서 처리하는 함수를 ISR함수라고 한다.
{
	uart0_rx_data= UDR0;	//UDR0 :	USART0 포트의 송수신 데이터 버퍼의 기능을 수행한다.
	uart0_rx_flag= 1;
}

void UART0_init(long baud)
{
	unsigned short ubrr = (F_CPU/ (8 * baud)-1);	//보레이트를 결정하는 공식 (비동기 2배속 모드)
	UBRR0H= (unsigned char)(ubrr >> 8);
	UBRR0L= (unsigned char)ubrr;
	
	UCSR0A= (1 << U2X0);	//비동기 2배속 모드
	//	UCSR0A 레지스터는 USART0 포트의 송수신 동작을 송수신 상태를 저장하는 기능을 수행한다.

	UCSR0B= (1 << RXCIE0) | (0 << TXCIE0) | (1<< RXEN0) | (1 << TXEN0);	
	//	UCSROB 레지스터는 USART0 포트의 송수신 동작을 제어
	//	RXCIE0, TXCIE0: 수신완료,송신완료 인터럽트를 개별적으로 허용하는 비트, RXEN0, TXEN0: 송신 데이터, 수신 데이터 레지스터 준비완료 인터럽트를 개별적으로 허용하는 비트
	UCSR0C= (1 << UCSZ01) | (1 << UCSZ00);
	//	UCSR0C 레지스터는 USART0 포트의 송수신 동작을 제어하는 기능
	//	UCSZ01: 전송 문자의 데이터 비트수를 설정 (8비트로 설정됨)
	stdout= &mystdout;
	//stdout: 표준 출력
}


void UART0_putchar(char c)
{
	if(c == '\n') UART0_putchar('\r');
	while(!(UCSR0A &(1 << UDRE0)));
	UDR0 =c;
}


void UART_putchar(char c, FILE * stream)
{
	if(c == '\n') UART_putchar('\r', stream);
	while(!(UCSR0A & (1<< UDRE0)));
	UDR0 =c;
}

int UART0_getchar(char *c)
{
	if(uart0_rx_flag == 0)
	{
		return 0;
	}
	else
	{
		*c= uart0_rx_data;
		uart0_rx_flag= 0;
		return 1;
	}
}

int UART0_gets(char *c)
{
	char *data;
	int i=0;
	if (UART0_getchar(data) == 0)
	{
		return 0;
	}
	else
	{
		if (*data == '\n')
		{
			c[i] = *data;
			return 1;
		}
		else
		{
			c[i] = *data;
			i++;
		}		
	}
}

void UART0_puts(char *c)
{
int i=0;
	while(1)
	{
		if(c[i] == NULL)
		{
			break;
		}
		else
		{
			UART0_putchar(c[i]);
			i++;
		}
	}
}



////////			AX			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define sbi(REG8,BITNUM) REG8 |= _BV(BITNUM)
static unsigned char uart0_flag;
static unsigned char uart0_data;
typedef unsigned char byte;
byte gbpTxBuffer[128];
byte gbpParameter[128];
byte TxPacket (byte bID,byte blnstruction,byte bParameterLength);
#define CHECK_TXD1_FINISH bit_is_set(UCSR1A,6)	// 비트가 세트인지 확인하는 매크로, UCSR1A의 6번비트는 송신을 다했으면 1
void uart_ax_transmit(unsigned char data);
char *data;

void uart_ax_transmit(unsigned char data)
{
	while(!(UCSR1A & (1<<UDRE1)));		//UDRE1 = 5  , 레지스터 UCSR1A의 5비트는 자동(전송준비가 되면 1이된다)

	UDR1=data;
}

void uart_ax_init(unsigned int ubrr)
{
	
	UBRR1H = (unsigned char)(ubrr>>8);    // USART1 포트의 송수신 속도를 설정하는 기능을 수행한다.
	UBRR1L = (unsigned char)ubrr;

	UCSR1A = 1<<U2X1;					// 비동기 2배속 모드 , U2X1==1
	UCSR1B = (1<<TXEN1);				// Enable receiver and transmitter(송,수신 가능하게 함) , RXEN1==4  , TXEN1==3
	UCSR1C = 3<<UCSZ10;					// Set frame format: 8data, 1stop bit    ,   UCSZ10==1
}

void All_Control_TEST(	unsigned int ID_number_1, int chose_1, unsigned int flexible_val_1,unsigned int flexible_1, unsigned int p_number_1, unsigned int AX_Speed_1,
						unsigned int ID_number_2, int chose_2, unsigned int flexible_val_2,unsigned int flexible_2, unsigned int p_number_2, unsigned int AX_Speed_2,
						unsigned int ID_number_3, int chose_3, unsigned int flexible_val_3,unsigned int flexible_3, unsigned int p_number_3, unsigned int AX_Speed_3,
						unsigned int ID_number_4, int chose_4, unsigned int flexible_val_4,unsigned int flexible_4, unsigned int p_number_4, unsigned int AX_Speed_4,
						unsigned int ID_number_5, int chose_5, unsigned int flexible_val_5,unsigned int flexible_5, unsigned int p_number_5, unsigned int AX_Speed_5,
						unsigned int ID_number_6, int chose_6, unsigned int flexible_val_6,unsigned int flexible_6, unsigned int p_number_6, unsigned int AX_Speed_6,
						unsigned int ID_number_7, int chose_7, unsigned int flexible_val_7,unsigned int flexible_7, unsigned int p_number_7, unsigned int AX_Speed_7,
						unsigned int ID_number_8, int chose_8, unsigned int flexible_val_8,unsigned int flexible_8, unsigned int p_number_8, unsigned int AX_Speed_8,
						unsigned int ID_number_9, int chose_9, unsigned int flexible_val_9,unsigned int flexible_9, unsigned int p_number_9, unsigned int AX_Speed_9,
						unsigned int ID_number_10, int chose_10, unsigned int flexible_val_10,unsigned int flexible_10, unsigned int p_number_10, unsigned int AX_Speed_10)

{
	unsigned int position_1 = 3.41*p_number_1;			//Change 0~300 to 0~1023
	unsigned int position_2 = 3.41*p_number_2;			//Change 0~300 to 0~1023
	unsigned int position_3 = 3.41*p_number_3;			//Change 0~300 to 0~1023
	unsigned int position_4 = 3.41*p_number_4;			//Change 0~300 to 0~1023
	unsigned int position_5 = 3.41*p_number_5;			//Change 0~300 to 0~1023
	unsigned int position_5a = 3.41*(300-p_number_5);	//Change 0~300 to 0~1023
	unsigned int position_6 = 3.41*p_number_6;			//Change 0~300 to 0~1023
	unsigned int position_7 = 3.41*p_number_7;			//Change 0~300 to 0~1023
	unsigned int position_8 = 3.41*p_number_8;			//Change 0~300 to 0~1023
	unsigned int position_9 = 3.41*p_number_9;			//Change 0~300 to 0~1023
	unsigned int position_10 = 3.41*p_number_10;		//Change 0~300 to 0~1023
	unsigned int position_10a = 3.41*(300-p_number_10);	//Change 0~300 to 0~1023

	unsigned int AX_Speed_10_c = 9*AX_Speed_10;
	unsigned int AX_Speed_9_c = 9*AX_Speed_9;
	unsigned int AX_Speed_8_c = 9*AX_Speed_8;
	unsigned int AX_Speed_7_c = 9*AX_Speed_7;
	unsigned int AX_Speed_6_c = 9*AX_Speed_6;
	unsigned int AX_Speed_5_c = 9*AX_Speed_5;
	unsigned int AX_Speed_4_c = 9*AX_Speed_4;
	unsigned int AX_Speed_3_c = 9*AX_Speed_3;
	unsigned int AX_Speed_2_c = 9*AX_Speed_2;
	unsigned int AX_Speed_1_c = 9*AX_Speed_1;
	
	int CheckSum_Value= 0;
	byte bCount,bCheckSum,bPacketLength;

	gbpTxBuffer[0] = 0xFF;
	gbpTxBuffer[1] = 0xFF;	//시작
	gbpTxBuffer[2] = 0xFE;	//ID

	gbpTxBuffer[3] = ((8 + 1)*12) + 4;	//길이 (L:Dynamixel별 Data Length, N:Dynamixel의 개수)

	gbpTxBuffer[4] = 0x83;	//명령

	gbpTxBuffer[5] =0x1a;
	gbpTxBuffer[6] =0x08;

	gbpTxBuffer[7] = 0x01;				//ID
	gbpTxBuffer[8] = flexible_val_1;	//유격 오차
	gbpTxBuffer[9] = flexible_val_1;	//유격 오차
	gbpTxBuffer[10] = flexible_1;		//유연성
	gbpTxBuffer[11] = flexible_1;		//유연성
	gbpTxBuffer[12] = (unsigned char)(position_1); //Writing Data  , goal position(L)
	gbpTxBuffer[13] = (unsigned char)(position_1>>8); //goal position(H)
	gbpTxBuffer[14] = (unsigned char)AX_Speed_1_c; //goal Speed(L)
	gbpTxBuffer[15] = (unsigned char)(AX_Speed_1_c >> 8); //goal Speed(H)

	gbpTxBuffer[16] = 0x02;				//ID
	gbpTxBuffer[17] = flexible_val_2;	//유격 오차
	gbpTxBuffer[18] = flexible_val_2;	//유격 오차
	gbpTxBuffer[19] = flexible_2;		//유연성
	gbpTxBuffer[20] = flexible_2;		//유연성
	gbpTxBuffer[21] = (unsigned char)(position_2); //Writing Data  , goal position(L)
	gbpTxBuffer[22] = (unsigned char)(position_2>>8); //goal position(H)
	gbpTxBuffer[23] = (unsigned char)AX_Speed_2_c; //goal Speed(L)
	gbpTxBuffer[24] = (unsigned char)(AX_Speed_2_c >> 8); //goal Speed(H)

	gbpTxBuffer[25] = 0x03;				//ID
	gbpTxBuffer[26] = flexible_val_3;	//유격 오차
	gbpTxBuffer[27] = flexible_val_3;	//유격 오차
	gbpTxBuffer[28] = flexible_3;		//유연성
	gbpTxBuffer[29] = flexible_3;		//유연성
	gbpTxBuffer[30] = (unsigned char)(position_3); //Writing Data  , goal position(L)
	gbpTxBuffer[31] = (unsigned char)(position_3>>8); //goal position(H)
	gbpTxBuffer[32] = (unsigned char)AX_Speed_3_c; //goal Speed(L)
	gbpTxBuffer[33] = (unsigned char)(AX_Speed_3_c >> 8); //goal Speed(H)

	gbpTxBuffer[34] = 0x04;				//ID
	gbpTxBuffer[35] = flexible_val_4;	//유격 오차
	gbpTxBuffer[36] = flexible_val_4;	//유격 오차
	gbpTxBuffer[37] = flexible_4;		//유연성
	gbpTxBuffer[38] = flexible_4;		//유연성
	gbpTxBuffer[39] = (unsigned char)(position_4); //Writing Data  , goal position(L)
	gbpTxBuffer[40] = (unsigned char)(position_4>>8); //goal position(H)
	gbpTxBuffer[41] = (unsigned char)AX_Speed_4_c; //goal Speed(L)
	gbpTxBuffer[42] = (unsigned char)(AX_Speed_4_c >> 8); //goal Speed(H)
	
	gbpTxBuffer[43] = 0x05;				//ID
	gbpTxBuffer[44] = flexible_val_5;	//유격 오차
	gbpTxBuffer[45] = flexible_val_5;	//유격 오차
	gbpTxBuffer[46] = flexible_5;		//유연성
	gbpTxBuffer[47] = flexible_5;		//유연성
	gbpTxBuffer[48] = (unsigned char)(position_5); //Writing Data  , goal position(L)
	gbpTxBuffer[49] = (unsigned char)(position_5>>8); //goal position(H)
	gbpTxBuffer[50] = (unsigned char)AX_Speed_5_c; //goal Speed(L)
	gbpTxBuffer[51] = (unsigned char)(AX_Speed_5_c >> 8); //goal Speed(H)
	
	gbpTxBuffer[52] = 0x06;				//ID
	gbpTxBuffer[53] = flexible_val_6;	//유격 오차
	gbpTxBuffer[54] = flexible_val_6;	//유격 오차
	gbpTxBuffer[55] = flexible_6;		//유연성
	gbpTxBuffer[56] = flexible_6;		//유연성
	gbpTxBuffer[57] = (unsigned char)(position_6); //Writing Data  , goal position(L)
	gbpTxBuffer[58] = (unsigned char)(position_6>>8); //goal position(H)
	gbpTxBuffer[59] = (unsigned char)AX_Speed_6_c; //goal Speed(L)
	gbpTxBuffer[60] = (unsigned char)(AX_Speed_6_c >> 8); //goal Speed(H)

	gbpTxBuffer[61] = 0x07;				//ID
	gbpTxBuffer[62] = flexible_val_7;	//유격 오차
	gbpTxBuffer[63] = flexible_val_7;	//유격 오차
	gbpTxBuffer[64] = flexible_7;		//유연성
	gbpTxBuffer[65] = flexible_7;		//유연성
	gbpTxBuffer[66] = (unsigned char)(position_7); //Writing Data  , goal position(L)
	gbpTxBuffer[67] = (unsigned char)(position_7>>8); //goal position(H)
	gbpTxBuffer[68] = (unsigned char)AX_Speed_7_c; //goal Speed(L)
	gbpTxBuffer[69] = (unsigned char)(AX_Speed_7_c >> 8); //goal Speed(H)

	gbpTxBuffer[70] = 0x08;				//ID
	gbpTxBuffer[71] = flexible_val_8;	//유격 오차
	gbpTxBuffer[72] = flexible_val_8;	//유격 오차
	gbpTxBuffer[73] = flexible_8;		//유연성
	gbpTxBuffer[74] = flexible_8;		//유연성
	gbpTxBuffer[75] = (unsigned char)(position_8); //Writing Data  , goal position(L)
	gbpTxBuffer[76] = (unsigned char)(position_8>>8); //goal position(H)
	gbpTxBuffer[77] = (unsigned char)AX_Speed_8_c; //goal Speed(L)
	gbpTxBuffer[78] = (unsigned char)(AX_Speed_8_c >> 8); //goal Speed(H)

	gbpTxBuffer[79] = 0x09;				//ID
	gbpTxBuffer[80] = flexible_val_9;	//유격 오차
	gbpTxBuffer[81] = flexible_val_9;	//유격 오차
	gbpTxBuffer[82] = flexible_9;		//유연성
	gbpTxBuffer[83] = flexible_9;		//유연성
	gbpTxBuffer[84] = (unsigned char)(position_9); //Writing Data  , goal position(L)
	gbpTxBuffer[85] = (unsigned char)(position_9>>8); //goal position(H)
	gbpTxBuffer[86] = (unsigned char)AX_Speed_9_c; //goal Speed(L)
	gbpTxBuffer[87] = (unsigned char)(AX_Speed_9_c >> 8); //goal Speed(H)

	gbpTxBuffer[88] = 0x0a;				//ID
	gbpTxBuffer[89] = flexible_val_10;	//유격 오차
	gbpTxBuffer[90] = flexible_val_10;	//유격 오차
	gbpTxBuffer[91] = flexible_10;		//유연성
	gbpTxBuffer[92] = flexible_10;		//유연성
	gbpTxBuffer[93] = (unsigned char)(position_10); //Writing Data  , goal position(L)
	gbpTxBuffer[94] = (unsigned char)(position_10>>8); //goal position(H)
	gbpTxBuffer[95] = (unsigned char)AX_Speed_10_c; //goal Speed(L)
	gbpTxBuffer[96] = (unsigned char)(AX_Speed_10_c >> 8); //goal Speed(H)

	gbpTxBuffer[97] = 0x0b;				//ID
	gbpTxBuffer[98] = flexible_val_5;	//유격 오차
	gbpTxBuffer[99] = flexible_val_5;	//유격 오차
	gbpTxBuffer[100] = flexible_5;		//유연성
	gbpTxBuffer[101] = flexible_5;		//유연성
	gbpTxBuffer[102] = (unsigned char)(position_5a); //Writing Data  , goal position(L)
	gbpTxBuffer[103] = (unsigned char)(position_5a>>8); //goal position(H)
	gbpTxBuffer[104] = (unsigned char)AX_Speed_10_c; //goal Speed(L)
	gbpTxBuffer[105] = (unsigned char)(AX_Speed_10_c >> 8); //goal Speed(H)

	gbpTxBuffer[106] = 0x0c;				//ID
	gbpTxBuffer[107] = flexible_val_10;	//유격 오차
	gbpTxBuffer[108] = flexible_val_10;	//유격 오차
	gbpTxBuffer[109] = flexible_10;		//유연성
	gbpTxBuffer[110] = flexible_10;		//유연성
	gbpTxBuffer[111] = (unsigned char)(position_10a); //Writing Data  , goal position(L)
	gbpTxBuffer[112] = (unsigned char)(position_10a>>8); //goal position(H)
	gbpTxBuffer[113] = (unsigned char)AX_Speed_10_c; //goal Speed(L)
	gbpTxBuffer[114] = (unsigned char)(AX_Speed_10_c >> 8); //goal Speed(H)

	for(int i=2; i<=114; i++)
	{
		CheckSum_Value+= gbpTxBuffer[i];
	}
	gbpTxBuffer[115] = ~(CheckSum_Value);	//Check Sum
	
	
	bPacketLength= 0x74;
	for(bCount=0;bCount<bPacketLength;bCount++)	//uart통신 Packet 전송
	{
		sbi(UCSR1A,6);
		uart_ax_transmit(gbpTxBuffer[bCount]);
	}
	while(!CHECK_TXD1_FINISH);		//전송이 끝날때 까지 대기
	return(bPacketLength);		//Packet길이 반환
}

void forward(void)
{
	All_Control_TEST(
	0x01, ON, 1, 32,	150,	100,
	0x02, ON, 1, 32,	225,	7,
	0x03, ON, 1, 32,	180,	15,
	0x04, ON, 1, 32,	130,	5,
	0x05, ON, 1, 32,	105,	100,
	
	0x06, ON, 1, 32,	150,	100,
	0x07, ON, 1, 32,	225,	7,
	0x08, ON, 1, 32,	180,	15,
	0x09, ON, 1, 32,	130,	5,
	0x0a, ON, 1, 32,	195,	100);
	_delay_ms(500);

	///오른쪽으로 기울이기
	All_Control_TEST(
	0x01, ON, 1, 32,	165,	4,
	0x02, ON, 1, 32,	225,	100,
	0x03, ON, 1, 32,	180,	100,
	0x04, ON, 1, 32,	130,	100,
	0x05, ON, 1, 32,	125,	4,
	
	0x06, ON, 1, 32,	165,	4,
	0x07, ON, 1, 32,	225,	100,
	0x08, ON, 1, 32,	180,	100,
	0x09, ON, 1, 32,	130,	100,
	0x0a, ON, 1, 32,	205,	4);
	_delay_ms(800);
			
	///왼쪽 다리 들기
	All_Control_TEST(
	0x01, ON, 1, 32,	165,	100,
	0x02, ON, 1, 32,	225,	100,
	0x03, ON, 1, 32,	200,	5,
	0x04, ON, 1, 32,	115,	4,
	0x05, ON, 1, 32,	125,	100,
	
	0x06, ON, 1, 32,	178,	3,
	0x07, ON, 1, 32,	225,	100,
	0x08, ON, 1, 32,	180,	100,
	0x09, ON, 1, 32,	130,	100,
	0x0a, ON, 1, 32,	205,	100);
	_delay_ms(1200);

	while(1)
	{
		///왼쪽 다리 앞으로
		All_Control_TEST(
		0x01, ON, 1, 32,	165,	100,
		0x02, ON, 1, 32,	258,	8,
		0x03, ON, 1, 32,	165,	8,
		0x04, ON, 1, 32,	105,	3,
		0x05, ON, 1, 32,	120,	3,
		
		0x06, ON, 1, 32,	178,	100,
		0x07, ON, 1, 32,	225,	100,
		0x08, ON, 1, 32,	180,	100,
		0x09, ON, 1, 32,	130,	100,
		0x0a, ON, 1, 32,	205,	100);
		_delay_ms(1000);

		///왼쪽 다리 앞으로
		All_Control_TEST(
		0x01, ON, 1, 32,	165,	100,
		0x02, ON, 1, 32,	258,	100,
		0x03, ON, 1, 32,	165,	100,
		0x04, ON, 1, 32,	105,	100,
		0x05, ON, 1, 32,	120,	100,
		
		0x06, ON, 1, 32,	178,	100,
		0x07, ON, 1, 32,	205,	6,
		0x08, ON, 1, 32,	200,	6,
		0x09, ON, 1, 32,	130,	100,
		0x0a, ON, 1, 32,	205,	100);
		_delay_ms(1000);

		///왼발 내딛기
		All_Control_TEST(
		0x01, ON, 1, 32,	153,	8,
		0x02, ON, 1, 32,	258,	100,
		0x03, ON, 1, 32,	165,	100,
		0x04, ON, 1, 32,	105,	100,
		0x05, ON, 1, 32,	115,	3,
		
		0x06, ON, 1, 32,	150,	4,
		0x07, ON, 1, 32,	220,	3,
		0x08, ON, 1, 32,	185,	3,
		0x09, ON, 1, 32,	130,	100,
		0x0a, ON, 1, 32,	195,	3);
		_delay_ms(1200);

		//무게중심 앞으로
		All_Control_TEST(
		0x01, ON, 1, 32,	153,	100,
		0x02, ON, 1, 32,	220,	9,
		0x03, ON, 1, 32,	190,	6,
		0x04, ON, 1, 32,	125,	5,
		0x05, ON, 1, 32,	115,	100,
			
		0x06, ON, 1, 32,	150,	100,
		0x07, ON, 1, 32,	220,	100,
		0x08, ON, 1, 32,	150,	8,
		0x09, ON, 1, 32,	170,	10,
		0x0a, ON, 1, 32,	195,	100);
		_delay_ms(1000);
				
		//무게중심 왼쪽
		All_Control_TEST(
		0x01, ON, 1, 32,	135,	4,
		0x02, ON, 1, 32,	225,	3,
		0x03, ON, 1, 32,	180,	3,
		0x04, ON, 1, 32,	130,	3,
		0x05, ON, 1, 32,	95,		5,
				
		0x06, ON, 1, 32,	137,	4,
		0x07, ON, 1, 32,	225,	3,
		0x08, ON, 1, 32,	150,	100,
		0x09, ON, 1, 32,	165,	3,
		0x0a, ON, 1, 32,	178,	6);
		_delay_ms(1000);

		//오른발 들기
		All_Control_TEST(
		0x01, ON, 1, 32,	124,	3,
		0x02, ON, 1, 32,	225,	100,
		0x03, ON, 1, 32,	180,	100,
		0x04, ON, 1, 32,	130,	100,
		0x05, ON, 1, 32,	90,		3,
		
		0x06, ON, 1, 32,	130,	3,
		0x07, ON, 1, 32,	210,	3,
		0x08, ON, 1, 32,	210,	15,
		0x09, ON, 1, 32,	120,	11,
		0x0a, ON, 1, 32,	170,	3);
		_delay_ms(1200);

		//오른발 앞으로
		All_Control_TEST(
		0x01, ON, 1, 32,	124,	100,
		0x02, ON, 1, 32,	225,	100,
		0x03, ON, 1, 32,	180,	100,
		0x04, ON, 1, 32,	130,	100,
		0x05, ON, 1, 32,	90,		100,
		
		0x06, ON, 1, 32,	130,	100,
		0x07, ON, 1, 32,	258,	13,
		0x08, ON, 1, 32,	165,	11,
		0x09, ON, 1, 32,	105,	3,
		0x0a, ON, 1, 32,	170,	100);
		_delay_ms(1000);

		//오른발 앞으로
		All_Control_TEST(
		0x01, ON, 1, 32,	124,	100,
		0x02, ON, 1, 32,	205,	6,
		0x03, ON, 1, 32,	200,	6,
		0x04, ON, 1, 32,	130,	100,
		0x05, ON, 1, 32,	90,		100,
				
		0x06, ON, 1, 32,	130,	100,
		0x07, ON, 1, 32,	258,	100,
		0x08, ON, 1, 32,	165,	100,
		0x09, ON, 1, 32,	105,	100,
		0x0a, ON, 1, 32,	170,	100);
		_delay_ms(1000);

		///오른발 내딛기
		All_Control_TEST(
		0x01, ON, 1, 32,	150,	3,
		0x02, ON, 1, 32,	220,	3,
		0x03, ON, 1, 32,	185,	3,
		0x04, ON, 1, 32,	130,	100,
		0x05, ON, 1, 32,	100,	3,
		
		0x06, ON, 1, 32,	147,	8,
		0x07, ON, 1, 32,	258,	100,
		0x08, ON, 1, 32,	165,	100,
		0x09, ON, 1, 32,	105,	100,
		0x0a, ON, 1, 32,	190,	3);
		_delay_ms(1200);

		//무게중심 앞으로
		All_Control_TEST(
		0x01, ON, 1, 32,	150,	100,
		0x02, ON, 1, 32,	220,	100,
		0x03, ON, 1, 32,	150,	8,
		0x04, ON, 1, 32,	170,	10,
		0x05, ON, 1, 32,	100,	100,

		0x06, ON, 1, 32,	147,	100,
		0x07, ON, 1, 32,	220,	9,
		0x08, ON, 1, 32,	190,	6,
		0x09, ON, 1, 32,	125,	5,
		0x0a, ON, 1, 32,	190,	100);
		_delay_ms(1000);
				
		//무게중심 오른쪽
		All_Control_TEST(
		0x01, ON, 1, 32,	175,	4,
		0x02, ON, 1, 32,	225,	3,
		0x03, ON, 1, 32,	150,	100,
		0x04, ON, 1, 32,	165,	3,
		0x05, ON, 1, 32,	130,	7,
		
		0x06, ON, 1, 32,	175,	4,
		0x07, ON, 1, 32,	225,	3,
		0x08, ON, 1, 32,	180,	3,
		0x09, ON, 1, 32,	130,	3,
		0x0a, ON, 1, 32,	215,	7);
		_delay_ms(1200);

		//왼발 들기
		All_Control_TEST(
		0x01, ON, 1, 32,	165,	3,
		0x02, ON, 1, 32,	210,	3,
		0x03, ON, 1, 32,	210,	15,
		0x04, ON, 1, 32,	120,	11,
		0x05, ON, 1, 32,	125,	3,
		
		0x06, ON, 1, 32,	178,	3,
		0x07, ON, 1, 32,	225,	100,
		0x08, ON, 1, 32,	180,	100,
		0x09, ON, 1, 32,	130,	100,
		0x0a, ON, 1, 32,	205,	3);
		_delay_ms(1200);

		if(UART0_gets(data) != 0)
		{
			All_Control_TEST(
			0x01, ON, 1, 32,	165,	100,
			0x02, ON, 1, 32,	225,	3,
			0x03, ON, 1, 32,	180,	7,
			0x04, ON, 1, 32,	130,	3,
			0x05, ON, 1, 32,	125,	100,
				
			0x06, ON, 1, 32,	165,	3,
			0x07, ON, 1, 32,	225,	100,
			0x08, ON, 1, 32,	180,	100,
			0x09, ON, 1, 32,	130,	100,
			0x0a, ON, 1, 32,	205,	100);
			_delay_ms(800);

			All_Control_TEST(
			0x01, ON, 1, 32,	150,	4,
			0x02, ON, 1, 32,	225,	100,
			0x03, ON, 1, 32,	180,	100,
			0x04, ON, 1, 32,	130,	100,
			0x05, ON, 1, 32,	105,	4,
					
			0x06, ON, 1, 32,	150,	4,
			0x07, ON, 1, 32,	225,	100,
			0x08, ON, 1, 32,	180,	100,
			0x09, ON, 1, 32,	130,	100,
			0x0a, ON, 1, 32,	195,	4);
			_delay_ms(800);
			break;
		}
	}
}




void backward(void)
{
	
}




void turnleft(void)
{
		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	150,	100,
		 		0x02, ON, 1, 32,	225,	7,
		 		0x03, ON, 1, 32,	180,	15,
		 		0x04, ON, 1, 32,	130,	5,
		 		0x05, ON, 1, 32,	105,	100,
		 		
		 		0x06, ON, 1, 32,	150,	100,
		 		0x07, ON, 1, 32,	225,	7,
		 		0x08, ON, 1, 32,	180,	15,
		 		0x09, ON, 1, 32,	130,	5,
		 		0x0a, ON, 1, 32,	195,	100);
		 		_delay_ms(500);

		 		//왼쪽으로 돌기
		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	150,	100,
		 		0x02, ON, 1, 32,	242,	10,
		 		0x03, ON, 1, 32,	180,	100,
		 		0x04, ON, 1, 32,	100,	14,
		 		0x05, ON, 1, 32,	105,	100,
		 		
		 		0x06, ON, 1, 32,	150,	100,
		 		0x07, ON, 1, 32,	204,	10,
		 		0x08, ON, 1, 32,	180,	100,
		 		0x09, ON, 1, 32,	150,	10,
		 		0x0a, ON, 1, 32,	195,	100);
		 		_delay_ms(500);

		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	128,	3,
		 		0x02, ON, 1, 32,	225,	5,
		 		0x03, ON, 1, 32,	180,	100,
		 		0x04, ON, 1, 32,	100,	100,
		 		0x05, ON, 1, 32,	90,		3,
		 		
		 		0x06, ON, 1, 32,	130,	3,
		 		0x07, ON, 1, 32,	204,	100,
		 		0x08, ON, 1, 32,	180,	100,
		 		0x09, ON, 1, 32,	150,	100,
		 		0x0a, ON, 1, 32,	180,	3);
		 		_delay_ms(1300);

		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	127,	3,
		 		0x02, ON, 1, 32,	225,	5,
		 		0x03, ON, 1, 32,	180,	100,
		 		0x04, ON, 1, 32,	130,	7,
		 		0x05, ON, 1, 32,	90,		100,
		 		
		 		0x06, ON, 1, 32,	130,	100,
		 		0x07, ON, 1, 32,	210,	3,
		 		0x08, ON, 1, 32,	210,	7,
		 		0x09, ON, 1, 32,	120,	7,
		 		0x0a, ON, 1, 32,	175,	3);
		 		_delay_ms(1000);

		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	135,	3,
		 		0x02, ON, 1, 32,	225,	100,
		 		0x03, ON, 1, 32,	180,	100,
		 		0x04, ON, 1, 32,	130,	100,
		 		0x05, ON, 1, 32,	90,		100,
		 		
		 		0x06, ON, 1, 32,	135,	3,
		 		0x07, ON, 1, 32,	225,	3,
		 		0x08, ON, 1, 32,	180,	7,
		 		0x09, ON, 1, 32,	130,	3,
		 		0x0a, ON, 1, 32,	175,	100);
		 		_delay_ms(800);

		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	150,	3,
		 		0x02, ON, 1, 32,	225,	100,
		 		0x03, ON, 1, 32,	180,	100,
		 		0x04, ON, 1, 32,	130,	100,
		 		0x05, ON, 1, 32,	105,	3,
		 		
		 		0x06, ON, 1, 32,	150,	3,
		 		0x07, ON, 1, 32,	225,	100,
		 		0x08, ON, 1, 32,	180,	100,
		 		0x09, ON, 1, 32,	130,	100,
		 		0x0a, ON, 1, 32,	195,	5);
		 		_delay_ms(500);

		 		if(UART0_gets(data) != 0)
		 		{
					return;
		 		}	
}




void turnright(void)
{
/*
		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	150,	100,
		 		0x02, ON, 1, 32,	225,	7,
		 		0x03, ON, 1, 32,	180,	15,
		 		0x04, ON, 1, 32,	130,	5,
		 		0x05, ON, 1, 32,	105,	100,
		 		
		 		0x06, ON, 1, 32,	150,	100,
		 		0x07, ON, 1, 32,	225,	7,
		 		0x08, ON, 1, 32,	180,	15,
		 		0x09, ON, 1, 32,	130,	5,
		 		0x0a, ON, 1, 32,	195,	100);
		 		_delay_ms(500);

		 		//오른쪽으로 돌기
		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	150,	100,
		 		0x02, ON, 1, 32,	204,	10,
		 		0x03, ON, 1, 32,	180,	100,
		 		0x04, ON, 1, 32,	150,	10,
		 		0x05, ON, 1, 32,	105,	100,
		 		
		 		0x06, ON, 1, 32,	150,	100,
		 		0x07, ON, 1, 32,	242,	10,
		 		0x08, ON, 1, 32,	180,	100,
		 		0x09, ON, 1, 32,	100,	14,
		 		0x0a, ON, 1, 32,	195,	100);
		 		_delay_ms(500);

		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	150,	3,
		 		0x02, ON, 1, 32,	204,	100,
		 		0x03, ON, 1, 32,	180,	100,
		 		0x04, ON, 1, 32,	150,	100,
		 		0x05, ON, 1, 32,	115,	3,
		 		
		 		0x06, ON, 1, 32,	168,	3,
		 		0x07, ON, 1, 32,	225,	5,
		 		0x08, ON, 1, 32,	180,	100,
		 		0x09, ON, 1, 32,	100,	100,
		 		0x0a, ON, 1, 32,	210,	3);
		 		_delay_ms(1500);

		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	140,	3,
		 		0x02, ON, 1, 32,	208,	3,
		 		0x03, ON, 1, 32,	180,	100,
		 		0x04, ON, 1, 32,	150,	100,
		 		0x05, ON, 1, 32,	115,	100,
		 		
		 		0x06, ON, 1, 32,	170,	3,
		 		0x07, ON, 1, 32,	227,	3,
		 		0x08, ON, 1, 32,	180,	100,
		 		0x09, ON, 1, 32,	100,	100,
		 		0x0a, ON, 1, 32,	210,	100);
		 		_delay_ms(300);

		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	160,	3,
		 		0x02, ON, 1, 32,	210,	3,
		 		0x03, ON, 1, 32,	210,	7,
		 		0x04, ON, 1, 32,	120,	7,
		 		0x05, ON, 1, 32,	125,	3,

		 		0x06, ON, 1, 32,	175,	3,
		 		0x07, ON, 1, 32,	227,	5,
		 		0x08, ON, 1, 32,	180,	100,
		 		0x09, ON, 1, 32,	130,	7,
		 		0x0a, ON, 1, 32,	210,	100);
		 		_delay_ms(1000);

		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	165,	3,
		 		0x02, ON, 1, 32,	225,	3,
		 		0x03, ON, 1, 32,	180,	7,
		 		0x04, ON, 1, 32,	130,	3,
		 		0x05, ON, 1, 32,	125,	100,
		 		
		 		0x06, ON, 1, 32,	165,	3,
		 		0x07, ON, 1, 32,	225,	3,
		 		0x08, ON, 1, 32,	180,	100,
		 		0x09, ON, 1, 32,	130,	100,
		 		0x0a, ON, 1, 32,	210,	100);
		 		_delay_ms(800);

		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	150,	3,
		 		0x02, ON, 1, 32,	225,	100,
		 		0x03, ON, 1, 32,	180,	100,
		 		0x04, ON, 1, 32,	130,	100,
		 		0x05, ON, 1, 32,	105,	3,
		 		
		 		0x06, ON, 1, 32,	150,	3,
		 		0x07, ON, 1, 32,	225,	100,
		 		0x08, ON, 1, 32,	180,	100,
		 		0x09, ON, 1, 32,	130,	100,
		 		0x0a, ON, 1, 32,	195,	5);
		 		_delay_ms(500);
		 		if(UART0_gets(data) != 0)
		 		{
			 		break;
		 		}	*/
}




void sideleft(void)
{
		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	150,	100,
		 		0x02, ON, 1, 32,	225,	7,
		 		0x03, ON, 1, 32,	180,	15,
		 		0x04, ON, 1, 32,	130,	5,
		 		0x05, ON, 1, 32,	105,	100,
		 		
		 		0x06, ON, 1, 32,	150,	100,
		 		0x07, ON, 1, 32,	225,	7,
		 		0x08, ON, 1, 32,	180,	15,
		 		0x09, ON, 1, 32,	130,	5,
		 		0x0a, ON, 1, 32,	195,	100);
		 		_delay_ms(500);

		 		///오른쪽으로 기울이기
		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	165,	4,
		 		0x02, ON, 1, 32,	225,	100,
		 		0x03, ON, 1, 32,	180,	100,
		 		0x04, ON, 1, 32,	130,	100,
		 		0x05, ON, 1, 32,	120,	4,
		 		
		 		0x06, ON, 1, 32,	165,	4,
		 		0x07, ON, 1, 32,	225,	100,
		 		0x08, ON, 1, 32,	180,	100,
		 		0x09, ON, 1, 32,	130,	100,
		 		0x0a, ON, 1, 32,	205,	4);
		 		_delay_ms(1000);

		 		///왼쪽 다리 들기
		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	165,	100,
		 		0x02, ON, 1, 32,	225,	100,
		 		0x03, ON, 1, 32,	200,	5,
		 		0x04, ON, 1, 32,	115,	4,
		 		0x05, ON, 1, 32,	120,	100,
		 		
		 		0x06, ON, 1, 32,	175,	3,
		 		0x07, ON, 1, 32,	225,	100,
		 		0x08, ON, 1, 32,	180,	100,
		 		0x09, ON, 1, 32,	130,	100,
		 		0x0a, ON, 1, 32,	205,	100);
		 		_delay_ms(1000);

		 		//왼쪽다리 내리기
		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	178,	4,
		 		0x02, ON, 1, 32,	241,	4,
		 		0x03, ON, 1, 32,	150,	12,
		 		0x04, ON, 1, 32,	150,	8,
		 		0x05, ON, 1, 32,	140,	3,
		 		
		 		0x06, ON, 1, 32,	175,	100,
		 		0x07, ON, 1, 32,	205,	5,
		 		0x08, ON, 1, 32,	205,	6,
		 		0x09, ON, 1, 32,	130,	100,
		 		0x0a, ON, 1, 32,	205,	100);
		 		_delay_ms(1200);

		 		//왼쪽으로 무게중심 이동
		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	124,	5,
		 		0x02, ON, 25, 128,	205,	6,
		 		0x03, ON, 1, 32,	205,	6,
		 		0x04, ON, 1, 32,	130,	3,
		 		0x05, ON, 1, 32,	90,		6,
		 		
		 		0x06, ON, 1, 32,	124,	6,
		 		0x07, ON, 1, 32,	241,	5,
		 		0x08, ON, 1, 32,	150,	4,
		 		0x09, ON, 1, 32,	150,	3,
		 		0x0a, ON, 1, 32,	160,	5);
		 		_delay_ms(2000);

		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	124,	100,
		 		0x02, ON, 1, 32,	225,	5,
		 		0x03, ON, 1, 32,	180,	6,
		 		0x04, ON, 1, 32,	130,	100,
		 		0x05, ON, 1, 32,	90,		100,
		 		
		 		0x06, ON, 1, 32,	130,	3,
		 		0x07, ON, 1, 32,	210,	7,
		 		0x08, ON, 1, 32,	210,	15,
		 		0x09, ON, 1, 32,	120,	7,
		 		0x0a, ON, 1, 32,	170,	3);
		 		_delay_ms(1000);

		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	135,	3,
		 		0x02, ON, 1, 32,	225,	100,
		 		0x03, ON, 1, 32,	180,	100,
		 		0x04, ON, 1, 32,	130,	100,
		 		0x05, ON, 1, 32,	95,		3,
		 		
		 		0x06, ON, 1, 32,	137,	3,
		 		0x07, ON, 1, 32,	225,	3,
		 		0x08, ON, 1, 32,	180,	7,
		 		0x09, ON, 1, 32,	130,	3,
		 		0x0a, ON, 1, 32,	178,	3);
		 		_delay_ms(1000);

		 		All_Control_TEST(
		 		0x01, ON, 1, 32,	150,	3,
		 		0x02, ON, 1, 32,	225,	100,
		 		0x03, ON, 1, 32,	180,	100,
		 		0x04, ON, 1, 32,	130,	00,
		 		0x05, ON, 1, 32,	105,	3,
		 		
		 		0x06, ON, 1, 32,	150,	3,
		 		0x07, ON, 1, 32,	225,	100,
		 		0x08, ON, 1, 32,	180,	100,
		 		0x09, ON, 1, 32,	130,	100,
		 		0x0a, ON, 1, 32,	195,	4);
		 		_delay_ms(500);

		 		if(UART0_gets(data) != 0)
		 		{
					return;
		 		}	
}




void sideright(void)
{
	All_Control_TEST(
	0x01, ON, 1, 32,	150,	100,
	0x02, ON, 1, 32,	225,	7,
	0x03, ON, 1, 32,	180,	15,
	0x04, ON, 1, 32,	130,	5,
	0x05, ON, 1, 32,	105,	100,
	
	0x06, ON, 1, 32,	150,	100,
	0x07, ON, 1, 32,	225,	7,
	0x08, ON, 1, 32,	180,	15,
	0x09, ON, 1, 32,	130,	5,
	0x0a, ON, 1, 32,	195,	100);
	_delay_ms(500);
	
	//왼쪽으로 무게중심 이동
	All_Control_TEST(
	0x01, ON, 1, 32,	135,	4,
	0x02, ON, 1, 32,	225,	100,
	0x03, ON, 1, 32,	180,	100,
	0x04, ON, 1, 32,	130,	100,
	0x05, ON, 1, 32,	95,		4,
	
	0x06, ON, 1, 32,	135,	4,
	0x07, ON, 1, 32,	225,	100,
	0x08, ON, 1, 32,	180,	100,
	0x09, ON, 1, 32,	130,	100,
	0x0a, ON, 1, 32,	180,	4);
	_delay_ms(1000);
	
	///오른쪽 다리 들기
	All_Control_TEST(
	0x01, ON, 1, 32,	128,	3,
	0x02, ON, 1, 32,	225,	100,
	0x03, ON, 1, 32,	180,	100,
	0x04, ON, 1, 32,	130,	100,
	0x05, ON, 1, 32,	95,		100,
	
	0x06, ON, 1, 32,	135,	100,
	0x07, ON, 1, 32,	225,	100,
	0x08, ON, 1, 32,	200,	5,
	0x09, ON, 1, 32,	115,	4,
	0x0a, ON, 1, 32,	180,	100);
	_delay_ms(1000);

	//오른쪽다리 내리기
	All_Control_TEST(
	0x01, ON, 1, 32,	128,	100,
	0x02, ON, 1, 32,	205,	5,
	0x03, ON, 1, 32,	205,	6,
	0x04, ON, 1, 32,	130,	100,
	0x05, ON, 1, 32,	95,		100,
	
	0x06, ON, 1, 32,	122,	4,
	0x07, ON, 1, 32,	241,	4,
	0x08, ON, 1, 32,	150,	12,
	0x09, ON, 1, 32,	150,	8,
	0x0a, ON, 1, 32,	160,	3);
	_delay_ms(1600);

	//오른쪽으로 무게중심 이동
	All_Control_TEST(
	0x01, ON, 1, 32,	176,	6,
	0x02, ON, 1, 32,	241,	5,
	0x03, ON, 1, 32,	150,	4,
	0x04, ON, 1, 32,	135,	3,
	0x05, ON, 1, 32,	120,	5,
	
	0x06, ON, 1, 32,	176,	5,
	0x07, ON, 10, 64,	205,	6,
	0x08, ON, 1, 32,	205,	6,
	0x09, ON, 1, 32,	115,	3,
	0x0a, ON, 1, 32,	205,	6);
	_delay_ms(2000);

	All_Control_TEST(
	0x01, ON, 1, 32,	170,	3,
	0x02, ON, 1, 32,	210,	7,
	0x03, ON, 1, 32,	210,	15,
	0x04, ON, 1, 32,	120,	7,
	0x05, ON, 1, 32,	125,	3,
	
	0x06, ON, 1, 32,	176,	100,
	0x07, ON, 1, 32,	225,	5,
	0x08, ON, 1, 32,	180,	6,
	0x09, ON, 1, 32,	130,	100,
	0x0a, ON, 1, 32,	205,	100);
	_delay_ms(1300);
	
	///오른쪽으로 기울이기
	All_Control_TEST(
	0x01, ON, 1, 32,	165,	3,
	0x02, ON, 1, 32,	225,	3,
	0x03, ON, 1, 32,	180,	7,
	0x04, ON, 1, 32,	130,	3,
	0x05, ON, 1, 32,	123,	3,
	
	0x06, ON, 1, 32,	165,	3,
	0x07, ON, 1, 32,	225,	100,
	0x08, ON, 1, 32,	180,	100,
	0x09, ON, 1, 32,	130,	100,
	0x0a, ON, 1, 32,	205,	3);
	_delay_ms(1000);
	
	All_Control_TEST(
	0x01, ON, 1, 32,	150,	3,
	0x02, ON, 1, 32,	225,	100,
	0x03, ON, 1, 32,	180,	100,
	0x04, ON, 1, 32,	130,	00,
	0x05, ON, 1, 32,	105,	4,
	
	0x06, ON, 1, 32,	150,	3,
	0x07, ON, 1, 32,	225,	100,
	0x08, ON, 1, 32,	180,	100,
	0x09, ON, 1, 32,	130,	100,
	0x0a, ON, 1, 32,	195,	3);
	_delay_ms(500);

	if(UART0_gets(data) != 0)
	{
		return;
	}
}




void sitdown(void)
{
			All_Control_TEST(
			0x01, ON, 1, 32,	150,	100,
			0x02, ON, 1, 32,	225,	6,
			0x03, ON, 1, 32,	180,	10,
			0x04, ON, 1, 32,	130,	11,
			0x05, ON, 1, 32,	105,	100,
			
			0x06, ON, 1, 32,	150,	100,
			0x07, ON, 1, 32,	225,	6,
			0x08, ON, 1, 32,	180,	10,
			0x09, ON, 1, 32,	130,	11,
			0x0a, ON, 1, 32,	195,	100);
			_delay_ms(1500);

			All_Control_TEST(
			0x01, ON, 1, 32,	150,	100,
			0x02, ON, 1, 32,	220,	6,
			0x03, ON, 1, 32,	225,	10,
			0x04, ON, 1, 32,	40,		11,
			0x05, ON, 1, 32,	105,	100,

			0x06, ON, 1, 32,	150,	100,
			0x07, ON, 1, 32,	220,	6,
			0x08, ON, 1, 32,	225,	10,
			0x09, ON, 1, 32,	40,		11,
			0x0a, ON, 1, 32,	195,	100);
			_delay_ms(1500);

			All_Control_TEST(
			0x01, ON, 1, 32,	150,	100,
			0x02, ON, 1, 32,	255,	10,
			0x03, ON, 1, 32,	225,	100,
			0x04, ON, 1, 32,	40,		100,
			0x05, ON, 1, 32,	105,	100,

			0x06, ON, 1, 32,	150,	100,
			0x07, ON, 1, 32,	255,	10,
			0x08, ON, 1, 32,	225,	100,
			0x09, ON, 1, 32,	40,		100,
			0x0a, ON, 1, 32,	195,	100);
			_delay_ms(2000);

			All_Control_TEST(
			0x01, ON, 1, 32,	150,	100,
			0x02, ON, 1, 32,	255,	100,
			0x03, ON, 1, 32,	225,	100,
			0x04, ON, 1, 32,	60,		5,
			0x05, ON, 1, 32,	105,	100,

			0x06, ON, 1, 32,	150,	100,
			0x07, ON, 1, 32,	255,	100,
			0x08, ON, 1, 32,	225,	100,
			0x09, ON, 1, 32,	60,		5,
			0x0a, ON, 1, 32,	195,	100);
			_delay_ms(1500);
			if(UART0_gets(data) != 0)
			{
//				break;
				return;
			}	
}




void standup(void)
{
			All_Control_TEST(
			0x01, ON, 1, 32,	150,	10,
			0x02, ON, 1, 32,	255,	10,
			0x03, ON, 1, 32,	225,	10,
			0x04, ON, 1, 32,	60,		10,
			0x05, ON, 1, 32,	105,	10,

			0x06, ON, 1, 32,	150,	10,
			0x07, ON, 1, 32,	255,	10,
			0x08, ON, 1, 32,	225,	10,
			0x09, ON, 1, 32,	60,		10,
			0x0a, ON, 1, 32,	195,	10);
			_delay_ms(500);


			All_Control_TEST(
			0x01, ON, 1, 32,	150,	100,
			0x02, ON, 1, 32,	255,	100,
			0x03, ON, 1, 32,	225,	100,
			0x04, ON, 1, 32,	40,		10,
			0x05, ON, 1, 32,	105,	100,

			0x06, ON, 1, 32,	150,	100,
			0x07, ON, 1, 32,	255,	100,
			0x08, ON, 1, 32,	225,	100,
			0x09, ON, 1, 32,	40,		10,
			0x0a, ON, 1, 32,	195,	100);
			_delay_ms(800);

			All_Control_TEST(
			0x01, ON, 1, 32,	150,	100,
			0x02, ON, 1, 32,	220,	10,
			0x03, ON, 1, 32,	225,	100,
			0x04, ON, 1, 32,	40,		100,
			0x05, ON, 1, 32,	105,	100,

			0x06, ON, 1, 32,	150,	100,
			0x07, ON, 1, 32,	220,	10,
			0x08, ON, 1, 32,	225,	100,
			0x09, ON, 1, 32,	40,		100,
			0x0a, ON, 1, 32,	195,	100);
			_delay_ms(1000);

			All_Control_TEST(
			0x01, ON, 1, 32,	150,	100,
			0x02, ON, 1, 32,	225,	6,
			0x03, ON, 1, 32,	180,	10,
			0x04, ON, 1, 32,	130,	11,
			0x05, ON, 1, 32,	105,	100,
			
			0x06, ON, 1, 32,	150,	100,
			0x07, ON, 1, 32,	225,	6,
			0x08, ON, 1, 32,	180,	10,
			0x09, ON, 1, 32,	130,	11,
			0x0a, ON, 1, 32,	195,	100);
			_delay_ms(1500);
			if(UART0_gets(data) != 0)
			{
//				break;
				return;
			}	
}




void ready(void)
{
			All_Control_TEST(
			0x01, ON, 1, 32,	150,	100,
			0x02, ON, 1, 32,	225,	7,
			0x03, ON, 1, 32,	180,	15,
			0x04, ON, 1, 32,	130,	12,
			0x05, ON, 1, 32,	105,	100,
			
			0x06, ON, 1, 32,	150,	100,
			0x07, ON, 1, 32,	225,	7,
			0x08, ON, 1, 32,	180,	15,
			0x09, ON, 1, 32,	130,	12,
			0x0a, ON, 1, 32,	195,	100);
			_delay_ms(500);	
}




void stand(void)
{
	All_Control_TEST(
	0x01, ON, 1, 32,	150,	100,
	0x02, ON, 1, 32,	240,	7,
	0x03, ON, 1, 32,	150,	15,
	0x04, ON, 1, 32,	150,	12,
	0x05, ON, 1, 32,	105,	100,

	0x06, ON, 1, 32,	150,	100,
	0x07, ON, 1, 32,	240,	7,
	0x08, ON, 1, 32,	150,	15,
	0x09, ON, 1, 32,	150,	12,
	0x0a, ON, 1, 32,	195,	100);
	_delay_ms(500);
}




/////////////////////	 타이머		////////////////////////////////////////////////////////////////////////////////////////////////

