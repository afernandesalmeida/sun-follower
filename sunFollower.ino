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
	
	while(1)
	{
		
	}
	
	return 0;
}