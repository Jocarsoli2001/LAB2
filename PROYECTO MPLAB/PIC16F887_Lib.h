// Declaración de funciones para que no importando su posición
// se puedan utilizar en cualquier otra función.
void PORTOut(char);

//funciones  --------------------------------------------------------------
#define RS RC0
#define RW RC3

#define D0 PORTBbits.RB0        // RD6 = D0
#define D1 PORTBbits.RB1        // RD7 = D1
#define D2 PORTBbits.RB2        // RB0 = D2
#define D3 PORTBbits.RB3        // RB1 = D3
#define D4 PORTBbits.RB4        // RB2 = D4
#define D5 PORTBbits.RB5        // RB3 = D5
#define D6 PORTBbits.RB6        // RB4 = D6
#define D7 PORTBbits.RB7        // RB5 = D7

#define EN RC1

// SETUP ADC -------------------------------------------------------------------


// INICIALIZAR LCD -------------------------------------------------------------
void InitLCD(char Lineas, char Alto){
    RS = 0; RW = 0;
    __delay_ms(14);                     // Esperar 15 ms (Mínimo) para que Vcc llegue a 4.5V
    PORTOut(0b00110000);                // Se envía el valor de D a los pines D0:D7 del LCD
    
    __delay_ms(4);                      // Esperar 4 ms (Mínimo)
    PORTOut(0b00110000);                // Bit 3: Bus de datos = 8 bits. 
                           
    __delay_ms(1);                      // Esperar 0.1 ms (Mínimo)
    PORTOut(0b00110000);                // Bit 3: Bus de datos = 8 bits.            
    
    // Setup de número de líneas y altura de caracter
    if (Lineas == 1 && Alto == 8){ PORTOut(0b00110000);}             // 1 Líneas | 8 Pixeles de alto = 00
    else if (Lineas == 2 && Alto == 11){ PORTOut(0b00111100);}       // 2 Líneas | 11 Pixeles de alto = 11
    else if (Lineas == 1 && Alto == 11){ PORTOut(0b00110100);}       // 1 Líneas | 11 Pixeles de alto = 01
    else if (Lineas == 2 && Alto == 8){ PORTOut(0b00111000);}        // 2 Líneas | 8 Pixeles de alto = 10                        
    
    PORTOut(0b00001000);                // Apagar Visualizador
    PORTOut(0b00000001);                // Borrar Visualizador
                    
    // Setup dirección de cursor
    // S (Bit 0): Shift activado = 1 | Shift desactivado = 0
    // ID (Bit 1): Cursor se mueve a la derecha = 1 | izquierda = 0
    PORTOut(0b00000110);  
    
    PORTOut(0b00001100);                // Encender Visualizador y no parpadeo de cursor (bit 0)
}

// CHEQUEAR SI SE PUEDE ESCRIBIR A LCD -----------------------------------------
// NOTA: Si se cambia el puerto asignado a D7, también se debe cambiar aquí
//char CheckLCD(){
//    char Listo;                 // Variable que indica si la LCD está lista para recibir comandos
//    
//    TRISBbits.TRISB5 = 1;       // Momentáneamente se cambia RB5 (D7) como input
//    RS = 0; RW = 1;             // Se activa la lectura de BF: Bandera que indica que la LCD está lista
//    
//    if(D7 == 0){ Listo = 1;}    // Si D7 = 0 entonces la LCD está lista
//    else { Listo = 0;}
//    
//    TRISBbits.TRISB5 = 0;       // Se regresa a RB5 (D7) como output
//    return(Listo);
//}

// MOVER CURSOR ----------------------------------------------------------------
void MoveCursor(char Linea, char Posiciones, char Direccion){
    RS = 0; RW = 0;                                 // Necesario para "Desplazar Cursor" y "Cambiar Dirección DDRAM"
    
    // Se puede escribir en la primera o segunda línea del LCD
    if (Linea == 1){ PORTOut(0b10000000);}          // Código = 1 (Comando para cambiar dirección DDRAM) 0000000 (Dirección = Posición 0). 
    else if (Linea == 2){ PORTOut(0b11000000);}     // Código = 1 (Comando para cambiar dirección DDRAM) 0101000 (Dirección = Posición 40)

    __delay_ms(40);                                 // Delay para permitir que la LCD finalice operaciones
    
    // Si se desea mover una posición (Posiciones > 1) entonces se mueve el cursor. Sino no se hace nada
    if (Posiciones != 0){
        
        // Si Direccion = 1 el cursor se mueve a la Derecha
        if (Direccion == 1){
            while(Posiciones > 0){
                PORTOut(0b00010100);                // Se desplaza 1 posición (0001) a la derecha (****01)
                Posiciones--;                       // Posiciones disminuye en 1
            }
        }
  
        // Si Direccion = 0 el cursor se mueve a la Izquierda
        else if(Direccion == 0){
            while(Posiciones > 0){    
                PORTOut(0b00010000);                // Se desplaza 1 posición (0001) a la izquierda (****00)
                Posiciones--;                       // Posiciones disminuye en 1
            }
        }

        __delay_ms(40);                             // Delay para permitir que la LCD finalice operaciones
    }
    
}

// ESCRIBIR CARACTERES EN LCD --------------------------------------------------
void WriteCharLCD(char *Caracter){      // NOTA: Caracter es un array de letras
    
    RS = 1; RW = 0;                 // Combinación requerida para escribir al LCD
    
    // El FOR realiza un ciclo por cada letra en el string hasta llegar al final del mismo ('\0')
    for (char i=0; Caracter[i]!='\0'; i++){   
        PORTOut(Caracter[i]);       // Envía el código ASCII de la letra en la posición i hacia los puertos D           
    }
}

// ESCRIBIR FLOATS EN LCD ------------------------------------------------------
//void WriteFloatLCD(int ConversionADC){
//    char Decenas, Unidades, Decimos, Centesimos, Milesimos;
//    Decenas = 0; Unidades = 0; Decimos = 0; Centesimos = 0; Milesimos = 0;
//    
//    for(int i=0; i < ConversionADC; i++){
//        // Cuenta el número de milésimos de 5 en 5.
//        // Esto se hace tomando en cuenta que el ADC sensa variaciones de 0.0049 V (Si VDD = 5 [VDD / 1023])
//        if (Milesimos == 5) { 
//            Milesimos = 0; Centesimos ++;               // Se resetea la cuenta de Milésimos
//            
//            if (Centesimos > 9) {                       // Si Centésimos > 9 luego del incremento previo
//                Centesimos = 0; Decimos++;
//                
//                if (Decimos > 9) {                      // Si Décimos > 9 luego del incremento previo
//                    Decimos = 0; Unidades++;
//                    
//                    if (Unidades > 9) {                 // Si Unidades > 9 luego del incremento previo
//                        Unidades = 0; Decenas++;        // NOTA: Decenas no se reseteará si sube arriba de 9.
//                    }
//                }
//            }
//            
//            
//        }
//        
//        else Milesimos = Milesimos + 5;
//
//    }
//    
//    RS = 1; RW = 0;                                     // Necesario para escribir en la LCD
//    
//    // El código ASCII para un dígito es:
//    // Nibble 1 = 0011
//    // Nibble 2 = Dígito en binario
//    PORTOut((Decenas & 0b00001111) + 0b00110000);       // Se escriben Decenas en LCD
//    PORTOut(Unidades + 0b00110000);                     // Se escriben Unidades en LCD
//    PORTOut(0b00101110);                                // Se escribe un punto decimal (Código entre paréntesis)
//    PORTOut(Decimos + 0b00110000);                      // Se escriben Décimos en LCD
//    PORTOut(Centesimos + 0b00110000);                   // Se escriben Centésimos en LCD
//    PORTOut(Milesimos + 0b00110000);                    // Se escriben Milésimos en LCD
//    
//}

// ESCRIBIR LOS VALORES DE D EN LOS PINES DE LA LCD ----------------------------
void PORTOut(char Out){
    if((Out & 0b00000001) == 0b00000001)  { D0 = 1; }  else { D0 = 0; }            // Si el bit 0 de D es 1: D0 se activa
    if((Out & 0b00000010) == 0b00000010)  { D1 = 1; }  else { D1 = 0; }            // Si el bit 1 de D es 1: D0 se activa
    if((Out & 0b00000100) == 0b00000100)  { D2 = 1; }  else { D2 = 0; }            // Si el bit 2 de D es 1: D0 se activa
    if((Out & 0b00001000) == 0b00001000)  { D3 = 1; }  else { D3 = 0; }            // Si el bit 3 de D es 1: D0 se activa
    if((Out & 0b00010000) == 0b00010000)  { D4 = 1; }  else { D4 = 0; }            // Si el bit 4 de D es 1: D0 se activa
    if((Out & 0b00100000) == 0b00100000)  { D5 = 1; }  else { D5 = 0; }            // Si el bit 5 de D es 1: D0 se activa
    if((Out & 0b01000000) == 0b01000000)  { D6 = 1; }  else { D6 = 0; }            // Si el bit 6 de D es 1: D0 se activa
    if((Out & 0b10000000) == 0b10000000)  { D7 = 1; }  else { D7 = 0; }            // Si el bit 7 de D es 1: D0 se activa
    
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

// LIMMPIAR LCD ----------------------------------------------------------------
void ResetLCD(){
    RS = 0; RW = 0;
    PORTOut(0b00000001);                        // Envío del comando de limpieza a los pines "D"
    __delay_ms(10);                             // Delay para permitir que la LCD finalice operaciones
    PORTOut(0b00000010);                        // Se coloca el cursor en la posición 1 de la primera línea
}