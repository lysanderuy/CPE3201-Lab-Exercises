#include <xc.h>

// CONFIGURATION BITS
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

unsigned int ADC = 0;
float voltage = 0.0;
unsigned int whole, decimal;
unsigned int result = 0x00;

void delay (int cnt) {
	while (cnt--);
}

void interrupt ISR(void)
{
	GIE = 0; // disable all unmasked interrupts (INTCON reg)
	int d_value = 0;
	
	if(ADIF==1) // checks CCP1 interrupt flag
	{
		ADIF = 0; // clears interrupt flag (INTCON reg)
		/* read result register */
		ADC = (ADRESH << 8) | ADRESL;
		voltage = ((ADC * 5.0)/1023.0);
		result =(unsigned int) (voltage*10);
		whole = result / 10;
		decimal = result % 10;
		
		PORTB = (((whole & 0x0F) << 4) | (decimal & 0x0F));
	}
	
	GIE = 1; // enable all unmasked interrupts (INTCON reg)
	delay(10);
	
	GO = 1; // restart A/D conversion (ADCON0 reg)
}

// Main function
void main() {
	int d_value = 0;

	TRISA = 0xFF;
	TRISB = 0x00; // set all PORTB as output
	PORTB = 0x00; // all LEDs OFF
	
	ADCON1 = 0x80; 	// result register: right Justified, clock: FOSC/8
					// all ports in PORTA are analog
					// VREF+=VDD, VREF-=VSS
	ADCON0 = 0x41; 	// clock: FOSC/8 analog channel: AN0
					// A/D conversion: STOP, A/D module: ON
	ADIF = 0;
	ADIE = 1;
	
	PEIE = 1;
	GIE = 1;
	
	delay(10);
	
	GO = 1;
					
	while (1) {
		
	}
}