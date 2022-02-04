/*
 * File:   USART.c
 * Author: Jos� Sanoli
 *
 * Created on 4 de febrero de 2022, 12:10
 */


#include <xc.h>
#include "USART.h"

void config_USART(void){
    //Configuraci�n de TX y RX
    SPBRG = 5;                                  // SPBRG:SPBRGH = 25
    SPBRGH = 0;
    
    TXSTAbits.SYNC = 0;                         // Transmisi�n as�ncrona
    TXSTAbits.BRGH = 0;                         // Baud rate a velocidad alta
    
    BAUDCTLbits.BRG16 = 0;                      // Baud rate de 16 bits
    
    RCSTAbits.SPEN = 1;                         // Puertos seriales habilitados
    RCSTAbits.RX9 = 0;                          // Recepci�n de datos de 8 bits 
    RCSTAbits.CREN = 1;                         // Recepci�n continua habilitada
    
    TXSTAbits.TXEN = 1;                         // Transmisiones habilitadas
}

void obtener_dato(void){
    dato = RCREG;
}

void transmitir_dato(void){
    
}


