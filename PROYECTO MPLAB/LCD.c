/*
 * File:   LCD.c
 * Author: José Sanoli
 *
 * Created on 31 de enero de 2022, 15:29
 */


#include "LCD.h"

void prender_ELCD(void){
    LCD_E = 1;                                  //Pulso a pin E para dejar pasar datos del data bus a la LCD
    __delay_ms(4);
    LCD_E = 0;
    __delay_ms(4);
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
    LCD_E = 0; 
    LCD_RS = 0; 
    LCD_databus = 0;
    LCD_E_direc = 0;
    LCD_RS_direc = 0;
    LCD_datos = 0;
    
    __delay_ms (16);
    Escribir_datosLCD(0b00110000);
    __delay_ms (5);
    Escribir_datosLCD(0b00110000);
    __delay_us (200);
    Escribir_datosLCD(0b00110000);
    
    // Reset
    __delay_ms (100);
    Escribir_datosLCD(0b00111000);
    __delay_ms (100);
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
