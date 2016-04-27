#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "ad.c"

// Portas
char c_PINO13 = PINB7;
char c_PINO12 = PINB6;
char c_PINO11 = PINB5;

// Flags p/ abertura e fechamento da porta (A) aberta, (F) fechada
int32_t i32_Flag_Bluetooth_A = 0;
int32_t i32_Flag_Bluetooth_F = 0;

// Flag p/ verificação do estado da lampada
int32_t i32_Flag_Lampada = 0;


void Uart_Init(void);
void Uart_Putchar(char c_Recebe);
char Uart_Getchar(void);
void Imprime_String(char c_Buffer_STR[ ], int32_t i32_N_Caracteres);
void Pwm_Init(void);
int32_t Ler_AD(int32_t i32_Flag_AD);


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

// Inicialização do UART
void Uart_Init(void)
{
  UBRR0H = 0;
  UBRR0L = 103; // Examples of UBRRn Settings for Commonly Used Oscillator Frequencies

  UCSR0A |= (1 << U2X0); //Writing this bit to one will double the transfer rate for asynchronous communication.
  UCSR0A &= ~(1 << U2X0);  // _BV(U2X0) = (1 << U2X0)      ( Pag. 195 )

  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); /* 8-bit data  */
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);   /* Enable RX and TX  */
}

// Enviar char através do UART
void Uart_Putchar(char c_Recebe)
{
  do
  {
  }
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = c_Recebe;
}

// Receber char através do UART
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

// Buffer para imprimir a String completa recebida do UART
void Imprime_String(char c_Buffer_STR[ ], int32_t i32_N_Caracteres)
{
  for (int i = 0; i < i32_N_Caracteres; i++)
  {
    Uart_Putchar(c_Buffer_STR[i]);
  }
}

// Inicialização do PWM
void Pwm_Init(void)
{
  DDRE |= (1 << DDE5); // PB1 is now output - OC3C

  TCCR3A |= (1 << COM3C1);// | (1 << COM3C0);

  TCCR3A |= (1 << WGM31) | (1 << WGM30);

  TCCR3B |= (1 << WGM32);//(1 << WGM33)

  TCCR3B |= (1 << CS31); //| (1 << CS30);
}

// Realiza a leitura do AD
int32_t Ler_AD(int32_t i32_Flag_AD)
{
  if (i32_Flag_AD == 0) //Realiza a leitura para a porta A0
  {
    //Varredura AD manualmente
    ADMUX &= ~(1 << MUX0);
    _delay_ms(50);
    int32_t i32_Valor_AD0 = ADCL;
    i32_Valor_AD0 = (ADCH << 8) + i32_Valor_AD0;

    return i32_Valor_AD0;
  }

  if (i32_Flag_AD == 1) //Realiza a leitura para a porta A1
  {
    ADMUX |= (1 << MUX0);
    _delay_ms(50);
    int   i32_Valor_AD1 = ADCL;
    i32_Valor_AD1 = (ADCH << 8) + i32_Valor_AD1;
    return i32_Valor_AD1;
  }
}