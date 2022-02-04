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