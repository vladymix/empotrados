#include <xc.h>
#include <stdlib.h>

#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF

void init_CAD();
void init_t0();
void __interrupt() t0int (void);

int numInterruptT0=0;
int dataADReady=0;


void __interrupt() t0int (void) //Interrupcion que saca el contenido por puertoB cada 500ns
{
    if(INTCONbits.T0IF){
         TMR0 =157;
         numInterruptT0++;
         if(numInterruptT0>=99){
           //GGO
            ADCON0bits.GO=1;
            numInterruptT0 = 0;
            
        }
        INTCONbits.T0IF=0; //Resetea la interrupción
    }
}

void init_t0()
{
    OPTION_REGbits.PS=0b111;
    OPTION_REGbits.T0CS=0;
    OPTION_REGbits.PSA=0;
    INTCONbits.T0IE=1;
}



int main() {
    PIE1bits.ADIE=1;    //Interrupcion CAD habilitada
    INTCONbits.PEIE=0;  //Interrupcion de periféricos habilitada        PREGUNTAR!!!!!!!!!!!!!!!!!
    INTCONbits.GIE=1;   //Interrupciones habilitadas
  
    init_t0();
    while(1){
        while(dataADReady){
             PORTB=ADRESL;
             dataADReady=0;
        }
    }
    
    return (EXIT_SUCCESS);
}