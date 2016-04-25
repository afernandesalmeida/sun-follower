#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "ad.c"

char c_PINO13 = PINB7;
char c_PINO12 = PINB6;
char c_PINO11 = PINB5;

int32_t i32_Flag_Bluetooth_A = 0;
int32_t i32_Flag_Bluetooth_F = 0;

int32_t i32_Flag_Lampada = 0;

void Uart_Init(void);
void Uart_Putchar(char c_Recebe);
char Uart_Getchar(void);


int main(void)
{
	
	/*SETANDO AS PORTAS COMO ENTRADA OU SAÍDA!! 0 = ENTRADA, 1 = SAÍDA*/
	DDRF &= ~(1 << DDF0); //ENTRADA DE SINAL LDR1 - REFERÊNCIA - PORTA A0
	DDRF &= ~(1 << DDF1); //ENTRADA DE SINAL LDR2 - EM BAIXO DO CANO - PORTA A1

	/*PORTAS DE ACIONAMENTO DO MOTOR E FINAIS DE CURSO!!*/
	DDRH |= (1 << DDH4); //SAÍDA DO MOTOR PARA GIRAR PARA ESQUERDA - PORTA 7
	DDRH |= (1 << DDH5); //SAÍDA DO MOTOR PARA GIRAR PARA DIREITA - PORTA 8

	DDRH &= ~(1 << DDH6); //ENTRADA SENSOR DE FIM DE CURSO 1 - INICIO - PORTA 9
	DDRB &= ~(1 << DDB4); //ENTRADA SENSOR DE FIM DE CURSO 2 - PORTA 10

	DDRB |= (1 << DDB7); // habilita o pino 13 para saída digital (OUTPUT).
	DDRB |= (1 << DDB6); // SETA O PINO 12 COMO SAÍDA
	DDRB |= (1 << DDB5); // SETA O PINO 11 COMO SAÍDA
	
	while(1)
	{
		
	}
	
	return 0;
}

void Uart_Init(void)
{
  UBRR0H = 0;
  UBRR0L = 103; // Examples of UBRRn Settings for Commonly Used Oscillator Frequencies

  UCSR0A |= (1 << U2X0); //Writing this bit to one will double the transfer rate for asynchronous communication.
  UCSR0A &= ~(1 << U2X0);  // _BV(U2X0) = (1 << U2X0)      ( Pag. 195 )

  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); /* 8-bit data  */
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);   /* Enable RX and TX  */
}

void Uart_Putchar(char c_Recebe)
{
  do
  {
  }
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = c_Recebe;
}

char Uart_Getchar(void)
{
  while (1)
  {
    bool K = (UCSR0A & (1 << RXC0));
    return UDR0;
    if (K == true)
    {
      break;
    }
  }
}