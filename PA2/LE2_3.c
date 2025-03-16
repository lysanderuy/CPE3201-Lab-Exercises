#include <xc.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void delay (int MUL) {
	int i,j;
	for (i=MUL; i != 0; i--) // loop until i = 0
	{
		for (j=0; j < 256; j++){} // loop until j = 0
	}
}

void instCtrl (unsigned char INST) {
	PORTB = INST; // load instruction to PORTB
	RC0 = 0; // set RS to 0 (instruction reg)
	RC1 = 0; // set RW to 0 (write)
	RC2 = 1; // set E to 1
	delay(1); // 1 ms delay (estimate)
	RC2 = 0; // set E to 0 (strobe)
}

void dataCtrl (unsigned char DATA) {
	PORTB = DATA; // load data to PORTB
	RC0 = 1; // set RS to 1 (data reg)
	RC1 = 0; // set RW to 0 (write)
	RC2 = 1; // set E to 1
	delay(1); // 1 ms delay (estimate)
	RC2 = 0; // set E to 0 (strobe)
}

initLCD() {
	delay(1); // LCD startup about 15ms
	instCtrl(0x38); // function set: 8-bit; dual-line
	instCtrl(0x08); // display off
	instCtrl(0x01); // display clear
	instCtrl(0x06); // entry mode: increment; shift off
	instCtrl(0x0E); // display on; cursor on; blink off
}

void main(void) {
    TRISB = 0x00;
    TRISC = 0x00;
    TRISD = 0xFF;
    int cnt = 80;    

    initLCD();
    
    while (1) {
        if (RD4 == 1) {  // Key pressed
            delay(5);
            if (RD4 == 1) {  // Check if still pressed
                switch (PORTD & 0x0F) {
                    case 0x00: dataCtrl('1'); break;
                    case 0x01: dataCtrl('2'); break;
                    case 0x02: dataCtrl('3'); break;
                    case 0x04: dataCtrl('4'); break;
                    case 0x05: dataCtrl('5'); break;
                    case 0x06: dataCtrl('6'); break;
                    case 0x08: dataCtrl('7'); break;
                    case 0x09: dataCtrl('8'); break;
                    case 0x0A: dataCtrl('9'); break;
                    case 0x0C: dataCtrl('*'); break;
                    case 0x0D: dataCtrl('0'); break;
                    case 0x0E: dataCtrl('#'); break;
                }
				cnt--;
            	delay(5);
            	if (cnt == 0) {
					initLCD();
					cnt = 80;
				} else if (cnt == 60) {
					instCtrl(0xC0);	
				} else if (cnt == 40) {
					instCtrl(0x94);	
				} else if (cnt == 20) {
					instCtrl(0xD4);	
				}
                while (RD4 == 1);  // Block until key is released
            }
        }    
    } 
}
