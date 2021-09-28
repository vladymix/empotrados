#include <xc.h>
#include <stdio.h>

// See /opt/microchip/xc8/v{ver}/docs/chips/16f886.html
// for details on #pragma config

#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF


void init_uart(void)
{  /* See chapter 12.3 PIC16F886 manual (table 12-5) */

  TXSTAbits.BRGH = ;
  BAUDCTLbits.BRG16 = ;

  // SPBRGH:SPBRG = 
  SPBRGH = ;
  SPBRG = ;  // 9600 baud rate with 20MHz Clock
  
  TXSTAbits.SYNC = ; /* Asynchronous */
  TXSTAbits.TX9 = ; /* TX 8 data bit */
  RCSTAbits.RX9 = ; /* RX 8 data bit */

  PIE1bits.TXIE = ; /* Disable TX interrupt */
  PIE1bits.RCIE = ; /* Disable RX interrupt */

  RCSTAbits.SPEN = ; /* Serial port enable */

  TXSTAbits.TXEN = ; /* Reset transmitter */
  TXSTAbits.TXEN = ; /* Enable transmitter */
  
 }

/* It is needed for printf */
void putch(char c)
{ 
  
 } 

void main(void)
{ OSCCON = 0b00001000; // External cristal
  init_uart();

  while(1)
    printf("Hola\r\n");
 }
