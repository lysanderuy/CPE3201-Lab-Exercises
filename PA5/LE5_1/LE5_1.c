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

void delay (int cnt) {
	while (cnt--);
}

int readADC () {
	int temp = 0;
	
	delay(1000); // delay to get the hold capacitor charged
	GO = 1; // start conversion
	while(GO_DONE==1); // wait for conversion to finish
	
	/* read result register */
	temp = ADRESH; // read ADRESH
	temp = temp << 8; // move to correct position
	temp = temp | ADRESL; // read ADRESL
	
	return temp;
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
	ADCON0 = 0x01; 	// clock: FOSC/8 analog channel: AN0
					// A/D conversion: STOP, A/D module: ON
					
	GO = 1;
					
	while (1) {
		d_value = readADC(); // get ADC value
		
		/* setting the LEDs */
		if(d_value>=0 && d_value<170) {
			PORTB = 0x00;
		}
		else if(d_value>=170 && d_value<341) {
			PORTB = 0x01;
		}
		else if(d_value>=341 && d_value<512) {
			PORTB = 0x03;
		}
		else if(d_value>=512 && d_value<683) {
			PORTB = 0x07;
		}
		else if(d_value>=683 && d_value<854) {
			PORTB = 0x0F;
		}
		else if(d_value>=854 && d_value<1025) {
			PORTB = 0x1F;
		}
		else {
			PORTB = 0x00;
		}
	}
}


