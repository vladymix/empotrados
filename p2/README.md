## TRMT = 1 
tenemos permisos para transmitir

// Para el simulador 
!PIR1.Bits.TXIF

while(!TXSATSBits.TRMT);
TXREG = 'a';


TXIF se pone a 1 cuando envia

Usar 
