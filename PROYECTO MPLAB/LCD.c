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
    LCD_E = 0;
    LCD_RS = 0;
    LCD_databus = 0;
    LCD_E_direc = 0;
    LCD_RS_direc = 0;
    LCD_datos = 0;
    
    Escribir_comandoLCD(0x38);
    Escribir_comandoLCD(0x0c);
    Escribir_comandoLCD(0x01);
    Escribir_comandoLCD(0x06);
}

void Escribir_stringLCD(const char *d){
    while(*d){
        Escribir_datosLCD(*d++);
    }
}

void Limpiar_pantallaLCD(void){
    Escribir_comandoLCD(0x01);
    __delay_ms(2);
}
