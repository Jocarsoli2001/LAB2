/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_H
#define	LCD_H

#include <xc.h> // include processor files - each processor file is guarded.

//----------------------------Constantes--------------------------------------
// Frecuencia para delay
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif


// Pines
#define LCD_E  RC1                   // Pin de Enable
#define LCD_RS RC0                   // Pin de Register select
#define LCD_databus PORTD            // Puerto que envia los datos a la pantalla

// Direcciones de pines
#define LCD_E_direc TRISC1           // Dirección de memoria del pin Enable
#define LCD_RS_direc TRISC0          // Dirección de memoria del pin RS
#define LCD_datos   TRISD            // Dirección de memoria del data bus


//-----------------------Variables utilizadas---------------------------------


//-----------------------Funciones definidas----------------------------------
void Escribir_comandoLCD(unsigned char);
void Escribir_datosLCD(char);
void Iniciar_LCD(void);
void Escribir_stringLCD(const char*);
void Limpiar_pantallaLCD(void);
void prender_ELCD(void);
void set_cursor(char a, char b);
void shift_right(void);
void shift_left(void);

#endif	/* ADC7seg_H */

