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

//---------------------Librearías creadas por usuario--------------------
#include "LCD.h"

//-----------------Definición de frecuencia de cristal---------------
#define _XTAL_FREQ 4000000

//-----------------------Constantes----------------------------------
#define  valor_tmr0 237                    // valor_tmr0 = 237

//-----------------------Variables------------------------------------


//------------Funciones sin retorno de variables----------------------
void setup(void);                           // Función de setup
void tmr0(void);                            // Función para reiniciar TMR0
void displays(void);                        // Función para alternar valores mostrados en displays

//-------------Funciones que retornan variables-----------------------

//----------------------Interrupciones--------------------------------
void __interrupt() isr(void){
    
}

//----------------------Main Loop--------------------------------
void main(void) {
    
    while(1){
        
    }
}

//----------------------Subrutinas--------------------------------
void setup(void){
    
    //Configuración de entradas y salidas
    ANSEL = 0b1000000000;                   // Pines 0 y 1 de PORTA como analógicos
    ANSELH = 0;
    
    TRISA = 0;                              // PORTB, bit 2 como entrada analógica
    TRISB = 0b0111;                         // PORB, bit 0 y 1 como entrada digital por resistencia pull up
    
    TRISC = 0;                              // PORTC como salida
    TRISD = 0;                              // PORTD como salida                           
    TRISE = 0;                              // PORTE como salida
    
    PORTA = 0;                              // Limpiar PORTA
    PORTD = 0;                              // Limpiar PORTD
    PORTC = 0;                              // Limpiar PORTC
    PORTE = 0;                              // Limpiar PORTE
    
    //Configuración de resistencias pull-up internas
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
    
    //Configuración de interrupt-on-change
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    INTCONbits.RBIF = 0;
    
    
    //Configuración de oscilador
    OSCCONbits.IRCF = 0b0110;               // Oscilador a 4 MHz = 110
    OSCCONbits.SCS = 1;
    
    //Configuración de TMR0
    OPTION_REGbits.T0CS = 0;                // bit 5  TMR0 Clock Source Select bit...0 = Internal Clock (CLKO) 1 = Transition on T0CKI pin
    OPTION_REGbits.T0SE = 0;                // bit 4 TMR0 Source Edge Select bit 0 = low/high 1 = high/low
    OPTION_REGbits.PSA = 0;                 // bit 3  Prescaler Assignment bit...0 = Prescaler is assigned to the Timer0
    OPTION_REGbits.PS2 = 1;                 // bits 2-0  PS2:PS0: Prescaler Rate Select bits
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
    TMR0 = valor_tmr0;                      // preset for timer register
    
    //Configuración del ADC
    ADCON1bits.ADFM = 0;                    // Resultado justificado a la izquierda
    ADCON1bits.VCFG0 = 0;                   // Voltaje 0 de referencia = VSS
    ADCON1bits.VCFG1 = 0;                   // Voltaje 1 de referencia = VDD
    
    ADCON0bits.ADCS = 0b01;                 // Conversión ADC generada a 2us
    ADCON0bits.CHS = 5;                     // Input Channel = AN0
    ADCON0bits.ADON = 1;                    // ADC = enabled
    __delay_us(50);
    
    //Configuración de interrupciones
    INTCONbits.T0IF = 0;                    // Habilitada la bandera de TIMER 0      
    INTCONbits.T0IE = 1;                    // Habilitar las interrupciones de TIMER 0
    INTCONbits.RBIF = 0;                    // Habilitada la bandera de interrupción de puerto B
    INTCONbits.RBIE = 1;                    // Habilitadas las interrupciones de puerto B
    INTCONbits.GIE = 1;                     // Habilitar interrupciones globales
    PIR1bits.ADIF = 0;                      // Limpiar bandera de interrupción del ADC
    PIE1bits.ADIE = 1;                      // Interrupción ADC = enabled
    INTCONbits.PEIE = 1;                    // Interrupciones periféricas activadas
    
    return;
}

void tmr0(void){
    INTCONbits.T0IF = 0;                    // Limpiar bandera de TIMER 0
    TMR0 = valor_tmr0;                      // TMR0 = 237
    return;
}

