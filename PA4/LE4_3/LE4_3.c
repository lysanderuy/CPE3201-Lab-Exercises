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

// Global variables to track interrupt flags
bit myTMR0IF = 0;
bit myINTF = 0;

// Interrupt Service Routine (ISR)
void interrupt ISR(void)
{
	GIE = 0; // disable all unmasked interrupts (INTCON reg)
	
	int period = 0;
	
	if(CCP1IF==1) // checks CCP1 interrupt flag
	{
		CCP1IF = 0; // clears interrupt flag
		TMR1 = 0; // resets TMR1
		period = CCPR1/1000; // transfers captured TMR1 value
		// normalize the value (make the number smaller)
		period = period*8; // multiply by the normalized TMR1 timeout
		RA0 ^= 1;
	}
	
	GIE = 1; // enable all unmasked interrupts (INTCON reg)
}

// Function to create a delay using Timer0 overflow count
void delay(int overflow_count) {
    int of_count = 0;
    while (of_count < overflow_count) {
        if (myTMR0IF == 1) {
            of_count++;
            myTMR0IF = 0;
        }
    }
}

// Main function
void main() {
	ADCON1 = 0x06;
	TRISA = 0x00;
	TRISC = 0x04; // set RC2 to input
	T1CON = 0x30; // 1:8 prescaler, Timer1 off
	CCP1CON = 0x05; // capture mode: every rising edge
	CCP1IE = 1; // enable TMR1/CCP1 match interrupt (PIE1 reg)
	CCP1IF = 0; // reset interrupt flag (PIR1 reg)
	PEIE = 1; // enable all peripheral interrupt (INTCON reg)
	GIE = 1; // enable all unmasked interrupts (INTCON reg)
	TMR1ON = 1; // Turns on Timer1 (T1CON reg)

    while (1) {
		
	}
}

