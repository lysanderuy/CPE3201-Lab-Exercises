#include <xc.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

bit myTMR0IF = 0;
unsigned char counter = 0x00;

void interrupt ISR() {
	GIE = 0;
	
    if (INTF == 1) {  
        INTF = 0;
        counter = PORTD & 0x0F; 
        if (counter > 0x09) {
        	counter = 0x00;
        }
        else {
        	
        }
        
        PORTC = counter;
    }
    
    if (TMR0IF == 1) {  
        TMR0IF = 0;
        myTMR0IF = 1;
    }
    
	GIE = 1;
}

void delay(int overflow_count) {
    int of_count = 0;
    while (of_count < overflow_count) {
        if (myTMR0IF == 1) {
            of_count++;
            myTMR0IF = 0;
        }
    }
}

void main() {
    TRISB = 0xFF;  
    TRISC = 0x00;  
    
    OPTION_REG = 0xC3;  
    
    INTF = 0;
    INTE = 1;  
    
    TMR0IF = 0;
    TMR0IE = 1;  
    
    GIE = 1; 

    PORTC = counter;  

    while (1) {
	    delay(195);
	    if (RB0 == 0) {
	    	counter = (counter + 1) % 10;
	    	PORTC = counter;
	    }
	}
}
