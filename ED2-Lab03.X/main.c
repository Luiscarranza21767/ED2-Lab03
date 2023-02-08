/* Universidad del Valle de Guatemala
 IE3054 Electrónica Digital 2
 Autor: Luis Pablo Carranza
 Compilador: XC8, MPLAB X IDE (v6.00)
 Proyecto: Laboratorio No.03
 Hardware PIC16F887
 Creado: 02/02/23
 Última Modificación: 02/02/23*/

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT // Oscillator Selection bits (INTOSC 
//oscillator without clock out)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and 
//can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR 
//pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code 
//protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code 
//protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/
//External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-
//Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin 
//has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit 
//(Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 
//(Write protection off)

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <stdio.h>
#include "SPI.h"
#include "LCD.h"
#include "oscilador.h"
#include "conversiones.h"

#define _XTAL_FREQ 2000000
void portsetup(void);

uint8_t lecADC;
uint8_t lecADC2;
int cont;
float conver;
float conver1;
char valADC[3];
char valADC2[3];
char uni;
char dec;
char cent;
/*
 * 
 */

void main(void) {
    setupINTOSC(5);     //Oscilador a 1MHz
    portsetup();
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1,2);
    Lcd_Write_String("S1:  S2:  S3:"); 
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("    V          V");
    SSPBUF = 0;
    
    while(1){
        __delay_ms(20);
        RA0 = 1;
         //Lectura de contador
        __delay_ms(10);
        
        RA0 = 0;
        //spiWrite(0);
        spiWrite(1);
        //RA0 = 1;
        while(!SSPSTATbits.BF);
        //RA0 = 0;
        cont = SSPBUF; // Guarda el dato de la lectura
        RA0 = 1;
        
        cent = inttochar(descomponer(2, cont));
        Lcd_Set_Cursor(2,7);
        Lcd_Write_Char(cent);
        dec = inttochar(descomponer(1, cont));
        Lcd_Set_Cursor(2,8);
        Lcd_Write_Char(dec);
        uni = inttochar(descomponer(0, cont));
        Lcd_Set_Cursor(2,9);
        Lcd_Write_Char(uni);
        
        //Lectura de ADC1
        __delay_ms(20);
        
        RA0 = 0;
        spiWrite(0);
        //RA0 = 1;
        while(!SSPSTATbits.BF);
        
        //RA0 = 0;
        lecADC = SSPBUF; // Guarda el dato de la lectura
        __delay_ms(10);
        
        RA0 = 1;
        
        // Mostrar datos de ADC1 en LCD
        conver = (lecADC*5.0)/255;
        sprintf(valADC, "%.2f", conver);
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(valADC);
        
        //Lectura de ADC2
        __delay_ms(20);
        
        RA1 = 0;
        spiWrite(0);
        while(!SSPSTATbits.BF);
        lecADC2 = spiRead();
        __delay_ms(10);
        RA1 = 1;
        
       
        __delay_ms(1);
        
        // Mostrar datos de ADC2 en LCD
        conver1 = (lecADC2*5.0)/255;
        sprintf(valADC2, "%.2f", conver1);
        Lcd_Set_Cursor(2,12);
        Lcd_Write_String(valADC2);
        
    }
}

void portsetup(){
    ANSEL = 0;
    ANSELH = 0;
    TRISD = 0;  // Puerto para LCD
    PORTD = 0;
    TRISB = 0;  // Puerto auxiliar
    PORTB = 0;
    TRISA = 0;  // Puerto para SS
    PORTA = 0;
//    TRISC4 = 1;
    PORTAbits.RA0 = 1;  // Deseleccionar esclavos
    PORTAbits.RA1 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}