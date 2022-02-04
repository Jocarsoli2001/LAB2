/*
 * File:   LAB2.c
 * Author: José Sanoli
 *
 * Created on 31 de enero de 2022, 15:03
 */


//---------------------Bits de configuración-------------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//-----------------Librerías utilizadas en código-------------------- 
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//---------------------Librearías creadas por usuario--------------------
#include "LCD.h"
#include "ADC.h"

//-----------------Definición de frecuencia de cristal---------------
#define _XTAL_FREQ 4000000

//-----------------------Constantes----------------------------------

//-----------------------Variables------------------------------------
char buffer1[10];
char buffer2[10];
char buffer3[10];
char vol1[];
char vol2[];
char vol3[];
uint8_t dato = 0;
uint8_t Cont_U = 0;

//------------Funciones sin retorno de variables----------------------
void setup(void);                                   // Función de setup
void conversion_char(void);                         // Función para convertir valores a caracteres
void divisor(uint8_t a, char dig[]);                // Función para dividir valores en dígitos y guardarlos en array
void dato_recibido(void);


//-------------Funciones que retornan variables-----------------------

//----------------------Interrupciones--------------------------------
void __interrupt() isr(void){
    if(PIR1bits.ADIF){                              // Interrupción de ADC
        ADC();                                      // Guarda valor de ADRESH en cont1 o cont2, dependiendo del canal seleccionado
        PIR1bits.ADIF = 0;                          // Apagar bandera de interrupción de ADC
    }
    if(PIR1bits.RCIF){
        dato = RCREG;
        
    }
    
}

//----------------------Main Loop--------------------------------
void main(void) {
    setup();
    ADCON0bits.GO = 1;                              // Comenzar conversión ADC
    while(1){
        // ---------------------Conversión de ADC-------------------------------
        conversion();                               // Genera conversión de analógico a digital dependiendo del canal seleccionado
        
        // -------------------Mostrar valores en LCD----------------------------
        set_cursor(1,1);                            // Setear cursor en 1,1 para la primera línea
        Escribir_stringLCD("S1:    S2:   S3:");     // Escribir menú en primera línea
        
        conversion_char();                          // Divide el valor de cont1 y cont2 en dígitos, los convierte a char y multiplica por 2 para verse como voltaje
        
        set_cursor(2,1);                            // Setear cursor en segunda línea
        Escribir_stringLCD(buffer1);                // Escribir valor de primer potenciómetro en LCD
        set_cursor(2,7);                            // Setear cursor en segunda línea, espacio 7
        Escribir_stringLCD(buffer2);                // Escribir valor de segundo potenciómetro
        
        // ---------------Mostrar valores de USART en LCD-----------------------
        dato_recibido();
        
        set_cursor(2,13);
        Escribir_stringLCD(buffer3);
    }
}

//----------------------Subrutinas--------------------------------
void setup(void){
    
    //Configuración de entradas y salidas
    ANSEL = 0b0011;                                 // Pines 0 y 1 de PORTA como analógicos
    ANSELH = 0;
    
    TRISA = 0b0011;                                 // PORTA, bit 0 y 1 como entrada analógica
    TRISB = 0;                                      // PORTB como salida
    TRISE = 0;                                      // PORTE como salida
    
    PORTA = 0;                                      // Limpiar PORTA
    PORTE = 0;                                      // Limpiar PORTE
    PORTB = 0;                                      // Limpiar PORTB
    
    //Configuración de oscilador
    OSCCONbits.IRCF = 0b0110;                       // Oscilador a 8 MHz = 111
    OSCCONbits.SCS = 1;
    
    //Configuración del ADC
    ADCON1bits.ADFM = 0;                            // Resultado justificado a la izquierda
    ADCON1bits.VCFG0 = 0;                           // Voltaje 0 de referencia = VSS
    ADCON1bits.VCFG1 = 0;                           // Voltaje 1 de referencia = VDD
    
    ADCON0bits.ADCS = 0b01;                         // Conversión ADC generada a 2us
    ADCON0bits.CHS = 0;                             // Input Channel = AN0
    ADCON0bits.ADON = 1;                            // ADC = enabled
    __delay_us(50);
    
    //Configuración de interrupciones
    PIR1bits.ADIF = 0;                              // Limpiar bandera de interrupción del ADC
    PIE1bits.ADIE = 1;                              // Interrupción ADC = enabled
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;                            // Interrupciones periféricas activadas
    INTCONbits.GIE = 1;                             // Habilitar interrupciones globales                        // Oscilador interno
    
    //Configuración de TX y RX
    TXSTAbits.SYNC = 0;                             // Transmisión asíncrona
    TXSTAbits.BRGH = 1;                             // Baud rate a velocidad baja
    
    BAUDCTLbits.BRG16 = 0;                          // Baud rate de 16 bits
    
    SPBRG = 23;                                      // SPBRG:SPBRGH = 25
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;                             // Puertos seriales habilitados
    RCSTAbits.RX9 = 0;                              // Recepción de datos de 8 bits 
    RCSTAbits.CREN = 1;                             // Recepción continua habilitada
    
    TXSTAbits.TXEN = 1;                             // Transmisiones habilitadas

    //Pantalla LCD
    Iniciar_LCD();                                  // Se inicializa la LCD en 8 bits
    Limpiar_pantallaLCD();                          // Se limpia la pantalla LCD
}

void conversion_char(void){
    divisor(cont1, vol1);                                            // Divide el valor de cont1 en dígitos
    sprintf(buffer1, "%d.%d%d", vol1[2], vol1[1], vol1[0]);          //Convierte el dato cont1 a string
    
    divisor(cont2, vol2);                                            // Divide el valor de cont2 en dígitos
    sprintf(buffer2, "%d.%d%d", vol2[2], vol2[1], vol2[0]);          //Convierte el dato cont2 a string
}

void divisor(uint8_t a, char dig[]){
    int b = 2*a;                                    // multiplica valores ingresados por 2
    for(int i = 0; i<3 ; i++){                      // De i = 0 hasta i = 2
        dig[i] = b % 10;                            // array[i] = cont_vol mod 10(retornar residuo). Devuelve digito por dígito de un número decimal.
        b = (b - dig[i])/10;                        // b = valor sin último digito.
    }
}

void dato_recibido(void){
    if(dato == 75){
        Cont_U++;
    }
    if(dato == 77){
        Cont_U--;
    }
    
    divisor(Cont_U, vol3);                                             // Divide el valor de cont2 en dígitos
    sprintf(buffer3, "%d.%d%d", vol3[2], vol3[1], vol3[0]);            //Convierte el dato cont2 a string
}
