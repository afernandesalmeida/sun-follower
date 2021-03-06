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

// Flag p/ verifica��o do estado da lampada
int32_t i32_Flag_Lampada = 0;


void Uart_Init(void);
void Uart_Putchar(char c_Recebe);
char Uart_Getchar(void);
void Imprime_String(char c_Buffer_STR[ ], int32_t i32_N_Caracteres);
void Pwm_Init(void);
int32_t Ler_AD(int32_t i32_Flag_AD);


int main(void)
{
	
	/*SETANDO AS PORTAS COMO ENTRADA OU SA�DA!! 0 = ENTRADA, 1 = SA�DA*/
	DDRF &= ~(1 << DDF0); //ENTRADA DE SINAL LDR1 - REFER�NCIA - PORTA A0
	DDRF &= ~(1 << DDF1); //ENTRADA DE SINAL LDR2 - EM BAIXO DO CANO - PORTA A1

	/*PORTAS DE ACIONAMENTO DO MOTOR E FINAIS DE CURSO!!*/
	DDRH |= (1 << DDH4); //SA�DA DO MOTOR PARA GIRAR PARA ESQUERDA - PORTA 7
	DDRH |= (1 << DDH5); //SA�DA DO MOTOR PARA GIRAR PARA DIREITA - PORTA 8

	DDRH &= ~(1 << DDH6); //ENTRADA SENSOR DE FIM DE CURSO 1 - INICIO - PORTA 9
	DDRB &= ~(1 << DDB4); //ENTRADA SENSOR DE FIM DE CURSO 2 - PORTA 10

	DDRB |= (1 << DDB7); // habilita o pino 13 para sa�da digital (OUTPUT).
	DDRB |= (1 << DDB6); // SETA O PINO 12 COMO SA�DA
	DDRB |= (1 << DDB5); // SETA O PINO 11 COMO SA�DA
	
	Uart_Init();
	Ad_Init();
	Pwm_Init();
	
	// Loop do sistema
	while(1)
	{
		// Abre e fecha port�o atrav�s do bluetooth
		char c_Dados_Bluetooth = Uart_Getchar();

		if ((c_Dados_Bluetooth == 'A') && (i32_Flag_Bluetooth_A != 1)) // Realiza a abertura do port�o
		{
		  i32_Flag_Bluetooth_F = 0;
		  PINB |= (0 << c_PINO12); // DESLIGA MOTOR
		  _delay_ms(50);
		  PINB |= (1 << c_PINO11); //LIGA MOTOR PARA ABRIR JANELA - PORTA 11
		  i32_Flag_Bluetooth_A = 1;

		  char c_Buf_Escrita[13] = {'P', 'o', 'r', 't', 'a', ' ', 'A', 'b', 'e', 'r', 't', 'a', '\n'};
		  Imprime_String(c_Buf_Escrita, 13);
		}

		if ((c_Dados_Bluetooth == 'F') && (i32_Flag_Bluetooth_F != 1)) //Realiza o fechamento do port�o
		{
		  i32_Flag_Bluetooth_A = 0;
		  PINB |= (0 << c_PINO11); // DESLIGA MOTOR
		  _delay_ms(50);
		  PINB |= (1 << c_PINO12); //LIGA MOTOR PARA FECHAR JANELA - PORTA 12
		  i32_Flag_Bluetooth_F = 1;

		  char c_Buf_Escrita[14] = {'P', 'o', 'r', 't', 'a', ' ', 'F', 'e', 'c', 'h', 'a', 'd', 'a', '\n'};
		  Imprime_String(c_Buf_Escrita, 14);
		}
		
		// Verifica as flags de estado do port�o p/ exibi��o atrav�s do bluetooth
		if (c_Dados_Bluetooth == 'S')
		{
		  char c_Buf_Escrita[19] = {'S', 'e', 'n', 's', 'o', 'r', ' ', 'R', 'e', 'f', 'e', 'r', 'e', 'n', 'c', 'i', 'a', ':', ' '};
		  Imprime_String(c_Buf_Escrita, 19);

		  if (Ler_AD(0) > 300)
		  {
			char c_Buf_Escrita[6] = {'A', 't', 'i', 'v', 'o', '\n'};
			Imprime_String(c_Buf_Escrita, 6);
		  }

		  if (Ler_AD(0) < 300)
		  {
			char c_Buf_Escrita[11] = {'D', 'e', 's', 'a', 't', 'i', 'v', 'a', 'd', 'o', '\n'};
			Imprime_String(c_Buf_Escrita, 11);
		  }

		  char c_Buf_Escrita_1[13] = {'S', 'e', 'n', 's', 'o', 'r', ' ', 'C', 'a', 'n', 'o', ':', ' '};
		  Imprime_String(c_Buf_Escrita_1, 13);

		  if (Ler_AD(1) > 300)
		  {
			char c_Buf_Escrita[6] = {'A', 't', 'i', 'v', 'o', '\n'};
			Imprime_String(c_Buf_Escrita, 6);
		  }

		  if (Ler_AD(1) < 300)
		  {
			char c_Buf_Escrita[11] = {'D', 'e', 's', 'a', 't', 'i', 'v', 'a', 'd', 'o', '\n'};
			Imprime_String(c_Buf_Escrita, 11);
		  }

		}
		
		// Controle de intensidade da lampada
		if (c_Dados_Bluetooth == 'L') //Intensidade zero!
		{
		  OCR3C = 0;
		  c_Dados_Bluetooth = 'Z';
		  i32_Flag_Lampada = 0;
		}

		if (c_Dados_Bluetooth == 'M') //Intensidade 50%
		{
		  OCR3C = 512;
		  c_Dados_Bluetooth = 'Z';
		  i32_Flag_Lampada = 512;
		}

		if (c_Dados_Bluetooth == 'N') //Intensidade 100%
		{
		  OCR3C = 1023;
		  c_Dados_Bluetooth = 'Z';
		  i32_Flag_Lampada = 1023;
		}

		if (c_Dados_Bluetooth == 'O') //Adiciona Intensidade
		{
		  if (i32_Flag_Lampada < 1003)
		  {
			i32_Flag_Lampada = i32_Flag_Lampada + 50;
			_delay_ms(50);
			OCR3C = i32_Flag_Lampada;
		  }

		  if (i32_Flag_Lampada > 1003)
		  {
			char d[14] = {'L', 'i', 'm', 'i', 't', 'e', ' ', 'M', 'a', 'x', 'i', 'm', 'o', '\n'};
			Imprime_String(d, 14);
		  }
		  c_Dados_Bluetooth = 'Z';
		}

		if (c_Dados_Bluetooth == 'P') //Reduz Intensidade
		{
		  if (i32_Flag_Lampada > 20)
		  {
			i32_Flag_Lampada = i32_Flag_Lampada - 50;
			_delay_ms(50);
			OCR3C = i32_Flag_Lampada;
		  }
		  if (i32_Flag_Lampada < 20)
		  {
			char d[14] = {'L', 'i', 'm', 'i', 't', 'e', ' ', 'M', 'i', 'n', 'i', 'm', 'o', '\n'};
			Imprime_String(d, 14);
		  }
		  c_Dados_Bluetooth = 'Z';
		}
	
	
		// Variaveis usadas no seguidor de sol
		bool b_Leitura_Digital_Pin_7 = (PINH & (1 << PINH4)); //DIGITALREAD PINO 7
		bool b_Leitura_Digital_Pin_8 = (PINH & (1 << PINH5)); //DIGITALREAD PINO 8
		bool b_Leitura_Digital_Pin_9 = (PINH & (1 << PINH6)); //DIGITALREAD PINO 9
		bool b_Leitura_Digital_Pin_10 = (PINB & (1 << PINB4)); //DIGITALREAD PINO 10
		
		// Seguidor do sol
		if (Ler_AD(0) > 600) //SE A REFERENCIA ESTIVER NO SOL E O FIM DE CURSO 2 N�O ATIVO!!!
		{

		  if ((Ler_AD(1) > 600) && (b_Leitura_Digital_Pin_8 != 1) && (b_Leitura_Digital_Pin_10 != 1) && (b_Leitura_Digital_Pin_7 != 1))
		  {
			PINH |= (1 << PINH5);
		  }
		  if ((Ler_AD(1) > 600) && (b_Leitura_Digital_Pin_8 == 1) && (b_Leitura_Digital_Pin_10 == 1))
		  {
			PINH |= (0 << PINH5);
			_delay_ms(50);
			PINH |= (1 << PINH4);
		  }

		  if ((Ler_AD(1) > 600) && (b_Leitura_Digital_Pin_7 == 1) && (b_Leitura_Digital_Pin_9 == 1))
		  {
			PINH |= (0 << PINH4);
			_delay_ms(50);
			PINH |= (1 << PINH5);
		  }

		  if (Ler_AD(1) < 600 && (b_Leitura_Digital_Pin_8 == 1 || b_Leitura_Digital_Pin_7 == 1))
		  {
			PINH |= (0 << PINH5);
			PINH |= (0 << PINH4);
		  }
		}

		if (Ler_AD(0) < 600) //SE A REFERENCIA ESTIVER NA SOMBRA!!
		{
		  if ((b_Leitura_Digital_Pin_9 != 1) && (b_Leitura_Digital_Pin_7 != 1))
		  {
			PINH |= (0 << PINH5);
			_delay_ms(50);
			PINH |= (1 << PINH4);
		  }
		  if ((b_Leitura_Digital_Pin_9 == 1) && (b_Leitura_Digital_Pin_7 == 1))
		  {
			PINH |= (0 << PINH4);
			_delay_ms(50);
			PINH |= (0 << PINH5);
		  }
		}
	
	
	}
	return 0;
}

// Inicializa��o do UART
void Uart_Init(void)
{
  UBRR0H = 0;
  UBRR0L = 103; // Examples of UBRRn Settings for Commonly Used Oscillator Frequencies

  UCSR0A |= (1 << U2X0); //Writing this bit to one will double the transfer rate for asynchronous communication.
  UCSR0A &= ~(1 << U2X0);  // _BV(U2X0) = (1 << U2X0)      ( Pag. 195 )

  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); /* 8-bit data  */
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);   /* Enable RX and TX  */
}

// Enviar char atrav�s do UART
void Uart_Putchar(char c_Recebe)
{
  do
  {
  }
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = c_Recebe;
}

// Receber char atrav�s do UART
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

// Inicializa��o do PWM
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