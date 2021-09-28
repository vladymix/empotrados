#include <xc.h>

// See /opt/microchip/xc8/v1.10/docs/chips/16f886.html
// for details on #pragma config

#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF

#pragma intrinsic(_delay)
#define _XTAL_FREQ 20000000 // necessary for __delay_us

void main(void)
{ char x,y;
  
  // OSCCON = 0b01110001;  // Change to internal 8MHz Clock
  OSCCON = 0b00001000; // External cristal

  x = 0;
  ANSEL = 0;	// Configure as digital all CAD channels
  ANSELH = 0;
  TRISA = 0; 	// Configure port A as output
  TRISB = 0;

  init_t0;
  
  while(1){   } 
}
void init_t0()
{
    OPTION_REGbits.PS = 0b111;
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
}
void __interrupt() t0mt(void)
{
    TMR0 = 157:
    x++;
    PORTB = x;
    INTCONbits.T0IF = 0;
}
