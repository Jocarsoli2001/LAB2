/*
 * File:   ADC.c
 * Author: José Sanoli
 *
 * Created on 4 de febrero de 2022, 9:01
 */


#include "ADC.h"

void ADC(void) {
    if(ADCON0bits.CHS == 0){
        cont1 = ADRESH;
    }
    else if(ADCON0bits.CHS == 1){
        cont2 = ADRESH;
    }
    
}

void conversion(void){
    if(ADCON0bits.GO == 0){             // Si bit GO = 0
        if(ADCON0bits.CHS == 1){        // Si Input Channel = AN1
            ADCON0bits.CHS = 0;         // Asignar input Channel = AN0
            __delay_us(50);             // Delay de 50 us
        }
        else if(ADCON0bits.CHS == 0){                           // Si Input Channel = AN0
            ADCON0bits.CHS = 1;         // Asignar Input Channel = AN1
            __delay_us(50);
        }
        __delay_us(50);
        ADCON0bits.GO = 1;              // Asignar bit GO = 1
    }
}

void config_ADC(int channel){
    ADCON1bits.ADFM = 0;                            // Resultado justificado a la izquierda
    ADCON1bits.VCFG0 = 0;                           // Voltaje 0 de referencia = VSS
    ADCON1bits.VCFG1 = 0;                           // Voltaje 1 de referencia = VDD
    
    ADCON0bits.ADCS = 0b01;                         // Conversión ADC generada a 2us
    ADCON0bits.CHS = channel;                       // Input Channel = AN0
    ADCON0bits.ADON = 1;                            // ADC = enabled
    __delay_us(50);
}