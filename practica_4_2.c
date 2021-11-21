#include <xc.h>
#include <stdlib.h>

#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF

void init_t0();
void init_t2();
void init_PWM1();
void interrupt interruption(void);

int numInterruptT0 = 0;
int dataADReady = 0;
int dutyCycle = 0;
int inverFlag = 0;

int useTimer0 = 1;

void interrupt interruption(void) //Interrupcion que saca el contenido por puertoB cada 500ns
{
    if (INTCONbits.T0IF)
    {
        TMR0 = 165; //Espera de 50 ms
        INTCONbits.T0IF = 0;
    }

    //Resetea la interrupción

    if (PIR1bits.TMR2IF)
    {

         if (CCPR2L == 255)
        {
            if (inverFlag == 0)
            {
                inverFlag = 1;
            }
            else
            {
                inverFlag = 0;
            }
        }
        
        if (inverFlag == 0)
        {
            CCPR2L++;
        }
        else
        {
            CCPR2L--;
        }

         CCPR1L = 166 - CCPR2L;


        TRISCbits.TRISC1 = 0; // Preguntar
        PIR1bits.TMR2IF = 0;
    }
}

void init_t0()
{
    OPTION_REGbits.PS = 0b111;
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    INTCONbits.T0IE = 1;
}

void init_t2()
{
    T2CONbits.TMR2ON = 1;
    T2CONbits.TOUTPS = 0; //Postescalado = 1:1
    T2CONbits.T2CKPS = 0; // Preescalado = 1:1
    PIE1bits.TMR2IE = 1;
    PIR1bits.TMR2IF = 0;
    PR2 = 166;
}

void init_PWM0()
{
   /*Init ccp1*/
    CCP1CONbits.P1M = 0;
    CCP1CONbits.DC1B = 0;
    CCP1CONbits.CCP1M = 0b1100; //Active high
    CCPR1L = 0;
}

void init_PWM1()
{
    TRISCbits.TRISC1 = 1; // Poner como entrada pin 1 de puerto c.
    PR2 = 165;            //Periodo del PWM

    CCP2CONbits.CCP2M = 0b1100; //Active high
    CCP2CONbits.DC2B0 = 0; //
    CCP2CONbits.DC2B1 = 0; //

     /*Init ccp2 igual*/
    CCPR2L = 255;
}

void interrupts()
{
    INTCONbits.PEIE = 1; //Interrupcion de periféricos habilitada PREGUNTAR!!!!!!!!!!!!!!!!!
    INTCONbits.GIE = 1;  //Interrupciones habilitadas
}

int main()
{
    PIE1bits.ADIE = 1;   //Interrupcion CAD habilitada
    
    OSCCON = 0b00001000; // External cristal
    TRISC = 0;
    interrups();
    init_t0();
    init_t2();
    init_PWM1();

    while (1);

    return (EXIT_SUCCESS);
}