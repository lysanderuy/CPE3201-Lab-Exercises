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

void blinkLED() {
	int cnt;
    for (int i = 0; i < 3; i++) {  
		for (cnt=0;cnt<10000;cnt++)
        	PORTBbits.RB0 = 1;  
		for (cnt=0;cnt<10000;cnt++)
       		PORTBbits.RB0 = 0;  
    }
}

void main() {
    // Configure I/O
    TRISB = 0x00;  
    TRISA = 0x01; 
    RB0 = 0;     

	if (RA0 == 1) { // Button Pressed (Active LOW)
		blinkLED(); // Blink LED 3 times
	}
}
