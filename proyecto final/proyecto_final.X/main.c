#include <xc.h>
#include <stdlib.h>
#include <stdio.h>

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

    //Resetea la interrupciÃ³n

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

void init_CAD(){
    //ConfiguraciÃ³n de puerto de entrada
    TRISA=1;    //Puerto A como entrada. Consultar a Norberto
    ANSEL=1;       //Establece entrada analogica. Idem
    //Puerto B de salida
    TRISB=0;
    //ConfiguraciÃ³n ADCON1
    ADCON1bits.ADFM=1;     //Justificado a la derecha.
    ADCON1bits.VCFG1=0;     //Ground como Vref-
    ADCON1bits.VCFG0=0;     //Vdd como Vref+    
    //ConfiguraciÃ³n ADCON0
    ADCON0bits.ADCS=0b10;   //Para configurar reloj a Fosc/32 (20Mhz, 1.6uS) PAGINA 10 ESPECIFICO
    ADCON0bits.CHS=0b0000;  //Selecciona entrada AN0
    ADCON0bits.ADON=1;      //Enciende conversor
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


void ShowDataToMonitor(){
    //
}

void CheckTempDial(){
    //Se añade al array entre 0-3 
}

int main()
{
    //Se inicia el programa
    PIE1bits.ADIE = 1;   //Interrupcion CAD habilitada
    INTCONbits.PEIE = 0; //Interrupcion de perifÃ©ricos habilitada PREGUNTAR!!!!!!!!!!!!!!!!!
    INTCONbits.GIE = 1;  //Interrupciones habilitadas
    
    //Inicializamos todo lo necesario para el correcto funcionamiento de HW2
    init_t0();
    init_t2();
    init_uart();
    init_CAD();
    init_PWM1();
    
    //Mostramos mensaje de bienvenida y 3 segundos despuès los datos de monitorización
    printf("Iniciando el Sistema...");
    //Esperar 3 segundos    
    while (1)
    {
        ShowDataToMonitor();//Se actualizará cada 1s. con los datos medios del último segundo
        CheckTempDial(); //Se verifica el dial de temperatura cada 0.25s|guardar últimos 4 datos en array
        
        while (dataADReady)
        {
            PORTB = ADRESL;
            dataADReady = 0;
        }
    }

    return (EXIT_SUCCESS);
}
