#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "ad.c"

char c_PINO13 = PINB7;
char c_PINO12 = PINB6;
char c_PINO11 = PINB5;

int32_t i32_Flag_Bluetooth_A = 0;
int32_t i32_Flag_Bluetooth_F = 0;