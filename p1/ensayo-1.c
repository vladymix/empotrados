#include <xc.h>

// See /opt/microchip/xc8/v1.10/docs/chips/16f886.html
// for details on #pragma config

#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF

#pragma intrinsic(_delay)
#define _XTAL_FREQ 20000000 // necessary for __delay_us



void init_t0();
void interrupt t0int (void);

int x=0;
int numInterruptT0=0;



void interrupt t0int (void)
{
    TMR0 = 157;
    numInterruptT0++;
    if(numInterruptT0>=199){
        x++;
        PORTB = x;
        numInterruptT0 = 0;
    }
    INTCONbits.T0IF=0; //Resetea la interrupción
}

void init_t0()
{
    OPTION_REGbits.PS=0b111;
    OPTION_REGbits.T0CS=0;
    OPTION_REGbits.PSA=0;
    INTCONbits.T0IE=1;
}

void main (void)
{
  // char x,y;

  // OSCCON = 0b01110001;  // Change to internal 8MHz Clock
  OSCCON = 0b00001000; // External cristal

  ANSEL = 0;	// Configure as digital all CAD channels
  ANSELH = 0;
  TRISA = 0; 	// Configure port A as output
  TRISB = 0;

  init_t0();
  
  INTCONbits.GIE=1;
  
  while(1){
  }
  
}