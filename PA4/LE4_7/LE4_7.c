#include<xc.h> // include file for the XC8 compiler
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

unsigned int period_flag = 0;
float period = 0.1;
unsigned int duty_flag = 0;
float percent = 0;
unsigned int result = 0;
float time = 0;

void interrupt ISR()
{

	GIE = 0;
	if(TMR2IF){
		TMR2IF = 0;		
	}

	GIE = 1;
}

/*void delay(int count)
{
	int OF_count = 0;
    
    while (OF_count < count)
    {
        if (TMR0IF_flag == 1)
        {
            OF_count++;
            TMR0IF_flag = 0;
        }
    }
}*/

void dutyCycle(){
	time = percent * period;
	result = (int)((time * 4000000)/16); 
	CCPR1L = result >> 2; 
	CCP1CON = ((result & 0x03) << 4) | 0x0C;	
}



void main()
{	
	TRISB = 0x03;
	TRISC = 0x00;
	
	// set value for PR2  
	PR2 = 0x1869;        
	
	CCPR1L = 0x71;     
	CCP1CON = 0x0C;    
     
	RC2 = 0;           
	T2CON = 0x06; 

	while(1)
	{
		if(RB0){  //PWM Period
			period_flag++;
			if(period_flag == 0){
				PR2 = 0x1869;
				period = 0.1;
			}
			else if(period_flag == 1){
				PR2 = 0x270;
				period = 0.01;
			}
			else if(period_flag == 2){
				PR2 = 0x3E;
				period = 0.001;
			}
			else if (period_flag > 2){
				period_flag = 0;
				period = 0.1;
				PR2 = 0x1869;
				
			}
			dutyCycle();
			while(RB0);	
		}
		
		if(RB1){
			duty_flag++;
			if(duty_flag == 0){
				percent = 0.1;
			}
			else if(duty_flag == 1){
				percent = 0.25;	
			}
			else if(duty_flag == 2){
				percent = 0.5;	
			}
			else if(duty_flag == 3){
				percent = 0.75;	
			}
			else if(duty_flag == 4){
				percent = 0.95;	 
			}
			else if(duty_flag > 4){
				duty_flag = 0;
				percent = 0.1; 	
			}
			dutyCycle();
			while(RB1);
		}
		
	}
}