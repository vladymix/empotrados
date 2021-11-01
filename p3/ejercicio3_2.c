/* 
 * File:   ejercicio1.c
 * Author: Grupo 4
 *
 * Created on 4 de octubre de 2021, 13:28
 */
#include <xc.h>
#include <stdlib.h>
#include <stdio.h>

#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF

void init_CAD();
void init_t0();
void __interrupt() t0int (void);

int numInterruptT0=0;
int puedoSeguir=0;
int dato=0;


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
    if (PIR1bits.ADIF){
        puedoSeguir=1;
        PIR1bits.ADIF=0;    //Flag de interrupción CAD a 0
    }
}

void init_CAD(){
    //Configuración de puerto de entrada
    TRISA=1;    //Puerto A como entrada. Consultar a Norberto
    ANSEL=1;       //Establece entrada analogica. Idem
    //Puerto B de salida
    TRISB=0;
    //Configuración ADCON1
    ADCON1bits.ADFM=1;     //Justificado a la derecha.
    ADCON1bits.VCFG1=0;     //Ground como Vref-
    ADCON1bits.VCFG0=0;     //Vdd como Vref+    
    //Configuración ADCON0
    ADCON0bits.ADCS=0b10;   //Para configurar reloj a Fosc/32 (20Mhz, 1.6uS) PAGINA 10 ESPECIFICO
    ADCON0bits.CHS=0b0000;  //Selecciona entrada AN0
    ADCON0bits.ADON=1;      //Enciende conversor
}

void init_t0()
{
    OPTION_REGbits.PS=0b111;
    OPTION_REGbits.T0CS=0;
    OPTION_REGbits.PSA=0;
    INTCONbits.T0IE=1;
}

void init_uart(void)
{  /* See chapter 12.3 PIC16F886 manual (table 12-5) */

  TXSTAbits.BRGH = 0; //Low speed asynchronous mode
  BAUDCTLbits.BRG16 = 0 ; // 8 bit mode

  // SPBRGH:SPBRG =
  SPBRGH = 0;
  SPBRG = 32;  // 9600 baud rate with 20MHz Clock . Actual rate = 9470. Err= -1.36%
  
  //Puertos 6 7 fisicos
  TRISCbits.TRISC6 = 1;
  TRISCbits.TRISC7 = 0;
    
  TXSTAbits.SYNC = 0; /* Asynchronous */
  TXSTAbits.TX9 = 0; /* TX 8 data bit  Para transmision de 8 bits*/
  // RCSTAbits.RX9 = 0; /* RX 8 data bit */

  PIE1bits.TXIE = 0; /* Disable TX interrupt */
  //PIE1bits.RCIE = ; /* Disable RX interrupt */

  RCSTAbits.SPEN = 1; /* Serial port enable */

  TXSTAbits.TXEN = 1; /* Modo transmitter */
  //TXSTAbits.TXEN = ; /* Enable transmitter */
 }

void putch(char c)
{

    // To real hardware
   /*
    while (!TRMT){
        continue;
    }
    */

    // To simulator
    while ( ! TXIF){
        continue;
    }
    TXREG = c;
 }

int main() {
    PIE1bits.ADIE=1;    //Interrupcion CAD habilitada
    INTCONbits.PEIE=0;  //Interrupcion de periféricos habilitada        PREGUNTAR!!!!!!!!!!!!!!!!!
    INTCONbits.GIE=1;   //Interrupciones habilitadas
    init_CAD(); //Configuracion de puertos y encendido del CAD
    init_t0();
    init_uart();
    while(1){
        //while(!PIR1bits.ADIF) {}  //Esperamos hasta que llegue interrupción de conversor A/D
    
        //while(!INTCONbits.T0IF){}                       //Espera de 500ms
    
        while(puedoSeguir){
             PORTB=ADRESL;
             dato=ADRESH;
             dato<<=8; //Cambio de 8 a 6
             dato|=ADRESL;
            
             printf("%d\n",dato);
             puedoSeguir=0;
        }
    }
    return (EXIT_SUCCESS);
}
