
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define F_CPU 14745600

#include <util/delay.h>
#include "_main.h"
#include "_glcd.h"
#include "_uart.h"

void Port_init(void) 						// 포트 초기화 구문입니다.
{
	PORTA = 0x00; 	DDRA = 0xFF;			// PORTA 출력 LOW ,핀의 출력 설정
	PORTB = 0xFE; 	DDRB = 0b11111110;			// PORTB 출력 LOW ,핀의 출력 설정
	PORTC = 0x00; 	DDRC = 0xF0;			// PORTC 상위 4bit 입력, 하위 4bit 출력
	PORTD = 0x80;	DDRD = 0b10000000;		// 핀의 입출력 설정
	PORTE = 0x00; 	DDRE = 0xFF;			// PORTE 출력 LOW ,핀의 출력 설정
	PORTF = 0x00; 	DDRF = 0x00;			// PORTE 출력 LOW ,핀의 출력 설정
}

void screen_display(void) { //게임 타이틀 화면
	lcd_clear(); //그래픽 LCD 클리어
	
	lcd_string(0,0,"====================");
	lcd_string(1,0,"  * Serial TEST *  "); //게임 이름
	lcd_string(2,0,"====================");
	
}


void Init_USART1(void)
{
    
    UCSR1B = 0x98;   // 1001 1000     
    //rxcie1(1, 수신 인터럽트 허가) txcie1(송신 인터럽트 허가) udrie1(usart전송완료 int허가 )    rxen1(1, 수신허가)  
    //txen1(1,송신허가) ucs221  rxb01(9비트 수신) txb01(9비트 송신, 9번째 비트저장)
	// RXCIE1=1(수신 인터럽트 설정 허가), RXEN1=1(수신 허가), TXEN1 = 1(송신 허가)
    UCSR1A = 0x00;      //1배속 전송모드
   //Rxcn(수신완료,rxcie1이 set시 int 발생) TXcn(송신완료,TXcie1이 set시 int발생)
   //UDREn(data 레지스터 준비완료, 1이면 data전송가능) FEn(수신버퍼 프레임 오류) DORn(overrun오류)
   //UPEn(패리티오류) U2Xn MPCMn(다중 프로세스)
   
   //URSEL1 : UCSR 레지스터 선택, 8비트 데이터 전송
    
    UCSR1C = 0x06;//비동기 통신, No Parity, 1 Stop Bit, 8데이터 비트 설정
    //UCSR1C = 0x06; 또는 UCSR1C = 0b000000110; 좌측과 같이 써넣어도 된다.
    
    //-UMSEL(0이면 비동기 전송), UPMn1, UPMn0(패리티 지정), USVSn(0이면 정지비트1)
    //UCSXN1, UCSZn0(문자길이), UCPLn

    UBRR1H = 0x00; //set baud rate hi
    UBRR1L = 0x07; // set baud rate lo    115200bps   
    sei();
}


void putch_USART1(char data)
{
 while( !(UCSR1A & (1<<UDRE1))); // 전송 인터럽트가 걸릴 때까지 while문 반복
 //UCSR1A가 1이 아닌동안 계속 루프를 돈다
 
 UDR1 = data; // 데이터 값을 UDR1에 넣는다. ⇒ 전송
}
                        
void puts_USART1(char *str)             // 문자열 출력 루틴
{                                                       
    while( *str != 0) {        // 문자의 마지막에는 ‘\0’이 들어 있으므로 
              putch_USART1(*str);    // ‘\0’이 나올 때까지 출력한다.
              str++;
    }
}


int main(void)                    // 여기서 부터 프로그램을 수행 합니다.
{

	char array[] = "Serial Comm Test \r\n";
	
	Port_init();             // 장치 초기화
	lcd_clear();
	lcd_init();                 // initialize GLCD
	screen_display();
	
	Init_USART1();
	while(1)
	{
		puts_USART1(array);
		_delay_ms(300);
	}

	
}
