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

void interrupt interruption(void) //Interrupcion que saca el contenido por puertoB cada 500ns
{
    if (INTCONbits.T0IF)
    {
        TMR0 = 165; //Espera de 50 ms
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

        INTCONbits.T0IF = 0;
    }

    //Resetea la interrupción

    if (PIR1bits.TMR2IF)
    {
        // setdutyCycle1(dutyCycle);
        TRISCbits.TRISC1 = 0;
        PIR1bits.TMR2IF = 0;
        //init_PWM1();
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
    PIR1bits.TMR2IF = 0;
    T2CONbits.T2CKPS = 0b00; //preescalado 1:1

    T2CONbits.TMR2ON = 1;
}

void init_PWM0()
{
    //TRISCbits.TRISC2 = 1; //Ponemos pin 2 puerto C como entrada.
    //PR2 = 165; //frecuencia de PWM a 30Khz
    //CCP1CONbits.
}
void init_PWM1()
{
    TRISCbits.TRISC1 = 1; // Poner como entrada pin 1 de puerto c.
    PR2 = 165;            //Periodo del PWM
    CCP2CONbits.CCP2M = 0b1100;
    CCP2CONbits.DC2B0 = 0; //
    CCP2CONbits.DC2B1 = 0; //
}

void setdutyCycle0(int dc)
{
}

// TODO Revisar esto
void setdutyCycle1(int dc)
{
    CCPR2L = (dutyCycle / 4) & 0x000000FF;
    CCP2CONbits.DC2B0 = dutyCycle & 0x00000001; //Revisar con Norberto
    CCP2CONbits.DC2B1 = dutyCycle & 0x00000002; //
}

int main()
{
    PIE1bits.ADIE = 1;   //Interrupcion CAD habilitada
    INTCONbits.PEIE = 0; //Interrupcion de periféricos habilitada PREGUNTAR!!!!!!!!!!!!!!!!!
    INTCONbits.GIE = 1;  //Interrupciones habilitadas
    init_t0();
    init_t2();
    init_PWM1();
    while (1)
    {
        while (dataADReady)
        {
            PORTB = ADRESL;
            dataADReady = 0;
        }
    }

    return (EXIT_SUCCESS);
}