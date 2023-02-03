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

#define _XTAL_FREQ 4000000
void portsetup(void);

uint8_t lecADC;
uint8_t lecADC2;
uint8_t cont;
float conver;
char valADC[3];
char contador[3];
/*
 * 
 */

void main(void) {
    setupINTOSC(6);     //Oscilador a 1MHz
    portsetup();
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1,2);
    Lcd_Write_String("S1:   S2:   S3:"); 
    
    while(1){
        
        PORTCbits.RC2 = 0;  // Seleccionar Slave 1
        PORTCbits.RC1 = 1;
        __delay_ms(1);
        spiWrite(0);
        lecADC = spiRead(); // Guarda el dato de la lectura
        
        PORTCbits.RC2 = 1;
        PORTCbits.RC1 = 0;
        __delay_ms(1);
        spiWrite(0);
        lecADC2 = spiRead();
        
//        PORTCbits.RC2 = 0;  // Seleccionar Slave 1
//        PORTCbits.RC1 = 1;
//        __delay_ms(1);   
//        spiWrite('c');
//        cont = spiRead();
//        PORTB = cont;
        
        
        conver = (lecADC*5.0)/255;
        sprintf(valADC, "%.2f", conver);
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(valADC);
        
        
//        sprintf(contador, "%.2f", cont);
//        Lcd_Set_Cursor(2,7);
//        Lcd_Write_String(contador);
        
        conver = (lecADC2*5.0)/255;
        sprintf(valADC, "%.2f", conver);
        Lcd_Set_Cursor(2,13);
        Lcd_Write_String(valADC);
        
    }
}

void portsetup(){
    ANSEL = 0;
    ANSELH = 0;
    TRISD = 0;
    PORTD = 0;
    TRISB = 0;
    PORTB = 0;

    TRISC = 0;
    TRISCbits.TRISC4 = 1;
    PORTCbits.RC2 = 1;  // Seleccionar Slave 1
    PORTCbits.RC1 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}