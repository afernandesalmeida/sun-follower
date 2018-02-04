#include <avr/io.h>

// constante para configuração do prescaler
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);


void Ad_Init(void) 
{

   // The recommended clock for the Atmega328 10 bits A/D converter is between 50KHz and 200 KHz. 
   // So, the only possible value is 128.
   // 16 MHz / 2 = 8 MHz
   // 16 MHz / 4 = 4 MHz
   // 16 MHz / 8 = 2 MHz
   // 16 MHz / 16 = 1 MHz
   // 16 MHz / 32 = 500 kHz
   // 16 MHz / 64 = 250 kHz
   // 16 MHz / 128 = 125 kHz
  
 // ADCSRA register ( Pag. 263 )
 // configura o preescaler do ADC
    ADCSRA |= PS_128; // 128 prescaler
 //  ADCSRA |= PS_64; // 64 prescaler
 //  ADCSRA |= PS_32; // 32 prescaler
 //  ADCSRA |= PS_16; // 16 prescaler

  // No Kit Arduíno, o pino 21 AREF não é conetado, por isso, 
  // ADMUX |= ( 1 << REFS1 ); // REFS1 = 0 e REFS0 = 1 (Pag. 262) (AVCC with external capacitor at AREF pin)
  ADMUX |= ( 1 << REFS0 );
  
  ADCSRA |= ( 1 << ADATE ); // Auto Trigger Enable  (Pag. 263)
  ADCSRA |= ( 1 << ADEN ); // Turn ON A/D converter
  ADCSRA |= ( 1 << ADSC ); // Start Convertion
}
