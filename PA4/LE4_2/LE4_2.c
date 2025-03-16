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
	
	if (TMR2IF==1) // checks Timer2 interrupt flag (TMR2=PR2)
	{
		TMR2IF = 0; // clears interrupt flag
		RA0 = RA0^1; // toggles the output signal at RA0
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
	ADCON1 = 0x06; // set all pins in PORTA as digital I/O
	TRISA = 0x00; // sets all of PORTA to output
	RA0 = 0; // initialize RA0 to 0
	T2CON = 0x01; // 1:4 prescaler, Timer2 off
	TMR2IE = 1; // enable Timer2/PR2 match interrupt (PIE1 reg)
	TMR2IF = 0; // reset interrupt flag (PIR1 reg)
	PEIE = 1; // enable all peripheral interrupt (INTCON reg)
	GIE = 1; // enable all unmasked interrupts (INTCON reg)
	PR2 = 0x7D; // match value for TMR2(125)at half cycle
	TMR2ON = 1; // Turns on Timer2 (T2CON reg)

    while (1) {
		
	}
}

