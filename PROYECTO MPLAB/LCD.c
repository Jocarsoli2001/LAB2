/*
 * File:   LCD.c
 * Author: José Sanoli
 *
 * Created on 31 de enero de 2022, 15:29
 */


#include "LCD.h"

void prender_ELCD(void){
    LCD_E = 1;                                  //Pulso a pin E para dejar pasar datos del data bus a la LCD
    __delay_us(500);
    LCD_E = 0;
    __delay_us(500);
}

void Escribir_comandoLCD(unsigned char Comando){
    LCD_RS = 0;
    LCD_databus = Comando;
    
    prender_ELCD();
}

void Escribir_datosLCD(char LCDchar){
    LCD_RS = 1;
    LCD_databus = LCDchar;
    
    prender_ELCD();
}

void Iniciar_LCD(void){
    LCD_RS = 0;
    LCD_RW = 0;
    
    __delay_ms (14);
    Escribir_datosLCD(0b00110000);
    __delay_ms (4);
    Escribir_datosLCD(0b00110000);
    __delay_us (100);
    Escribir_datosLCD(0b00110000);
    
    // Reset
    __delay_us (100);
    Escribir_datosLCD(0b00111000);                      // 2 lineas y espacios de 5x8 pixeles
    __delay_us (100);
    Escribir_datosLCD(0b00001000);                      // Apagar visualizador
    __delay_us (100);
    Escribir_datosLCD(0b00000001);                      // Borrar visualizador
    __delay_us (100);
    Escribir_datosLCD(0b00000110);                      // Modo de entrada con incremento y desplazamiento desactivado
    __delay_us (100);
    Escribir_datosLCD(0b00001100);                      
    return;
}

void Escribir_stringLCD(const char *d){
    while(*d){
        Escribir_datosLCD(*d++);
    }
}

void Limpiar_pantallaLCD(void){
    Escribir_comandoLCD(0x01);
    __delay_ms(4);
}

void set_cursor(char a, char b){
    if(a == 1){
        Escribir_comandoLCD(0x80 + (b-1));
    }
    if(a == 2){
        Escribir_comandoLCD(0xC0 + (b-1));
    }
    
}

void shift_right(void){
    Escribir_comandoLCD(0x01);
    Escribir_comandoLCD(0x0C);
}

void shift_left(void){
    Escribir_comandoLCD(0x01);
    Escribir_comandoLCD(0x08);
}
