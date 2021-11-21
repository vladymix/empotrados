#include <xc.h>
#include <stdlib.h>

#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF
#pragma intrinsic(_delay) // Preguntar


#define _XTAL_FREQ 20000000 // necessary for __delay_usint contador;

int ascendente;

void interrupt TMR2_int(void)
{
    if (PIR1bits.TMR2IF)
    {
        //si ha pasado el tiempo entra
        if (contador == 1500)
        {
            contador = 0;
            if (CCPR1L == 166 && ascendente)
            {
                ascendente = 0;
            }
            else if (CCPR1L == 0 && !ascendente)
            {
                ascendente = 1;
            }
            if (ascendente)
                CCPR1L++;
            else
                CCPR1L--;
            CCPR2L = 166 - CCPR1L;
        }
        else
            contador++;
        PIR1bits.TMR2IF = 0;
    }
}
void init_TMR2()
{
    T2CONbits.TMR2ON = 1;
    T2CONbits.TOUTPS = 0; //Postescalado = 1:1
    T2CONbits.T2CKPS = 0; // Preescalado = 1:1
    PIE1bits.TMR2IE = 1;
    PIR1bits.TMR2IF = 0;
    PR2 = 166;
}
void init_PWM()
{ /*Init ccp1*/
    CCP1CONbits.P1M = 0;
    CCP1CONbits.DC1B = 0;
    CCP1CONbits.CCP1M = 0b1100; //Active high
    CCPR1L = 0;
    /*Init ccp2 igual*/
    CCP2CONbits.DC2B0 = 0;
    CCP2CONbits.DC2B1 = 0;
    CCP2CONbits.CCP2M = 0b1100; //Active high
    CCPR2L = 166;
}
void interrupts()
{
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
}
void main(void)
{
    OSCCON = 0b00001000; // External cristal
    TRISC = 0;
    contador = 0;
    ascendente = 1;
    init_TMR2();
    init_PWM();
    interrupts();
    while (1)
        ;
}
