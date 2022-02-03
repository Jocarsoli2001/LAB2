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

//-----------------Definición de frecuencia de cristal---------------
#define _XTAL_FREQ 4000000

//-----------------------Constantes----------------------------------
#define  valor_tmr0 237                    // valor_tmr0 = 237

//-----------------------Variables------------------------------------
int num = 0;
char buffer[10];

//------------Funciones sin retorno de variables----------------------
void setup(void);                           // Función de setup
void tmr0(void);                            // Función para reiniciar TMR0
void displays(void);                        // Función para alternar valores mostrados en displays
void botones(void);


//-------------Funciones que retornan variables-----------------------

//----------------------Interrupciones--------------------------------
void __interrupt() isr(void){
    
    
}

//----------------------Main Loop--------------------------------
void main(void) {
    setup();
    Iniciar_LCD();                          // Initialize LCD in 8bit mode
    Limpiar_pantallaLCD();
    set_cursor(1,1);
    Escribir_stringLCD("S1");
    while(1){
        
    }
}

//----------------------Subrutinas--------------------------------
void setup(void){
    
    //Configuración de entradas y salidas
    ANSEL = 0;                              // Pines 0 y 1 de PORTA como analógicos
    ANSELH = 0;
    
    TRISA = 0b0011;                         // PORTB, bit 2 como entrada analógica
    TRISB = 0;                              // PORB, bit 0 y 1 como entrada digital por resistencia pull up
    TRISE = 0;                              // PORTE como salida
    
    PORTA = 0;                              // Limpiar PORTA
    PORTB = 0;
    
    //Configuración de oscilador
    OSCCONbits.IRCF = 0b0110;               // Oscilador a 4 MHz = 110
    OSCCONbits.SCS = 1;
    
    //Configuración del ADC
    ADCON1bits.ADFM = 0;                    // Resultado justificado a la izquierda
    ADCON1bits.VCFG0 = 0;                   // Voltaje 0 de referencia = VSS
    ADCON1bits.VCFG1 = 0;                   // Voltaje 1 de referencia = VDD
    
    ADCON0bits.ADCS = 0b01;                 // Conversión ADC generada a 2us
    ADCON0bits.CHS = 5;                     // Input Channel = AN0
    ADCON0bits.ADON = 1;                    // ADC = enabled
    __delay_us(50);
    
    //Configuración de interrupciones
    
    PIR1bits.ADIF = 0;                      // Limpiar bandera de interrupción del ADC
    PIE1bits.ADIE = 1;                      // Interrupción ADC = enabled
    INTCONbits.PEIE = 1;                    // Interrupciones periféricas activadas
    INTCONbits.GIE = 1;                     // Habilitar interrupciones globales

}

void botones(void){
    if(RA0){
        while(RA0);
        PORTB++;
    }
    else if(RA1){
        while(RA1);
        PORTB--;
    }
}
