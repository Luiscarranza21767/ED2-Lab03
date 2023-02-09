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
#include "oscilador.h"
#include "setupADC.h"

#define _XTAL_FREQ 2000000
#define READ_ADC 0
#define READ_CONT 1

void portsetup(void);
void setup_portb(void);

uint8_t lecADC;
uint8_t cont;

char dato;

/*
 * 
 */
void __interrupt() isr (void){
    if(SSPIF == 1){
        uint8_t com;
        com = spiRead();
        PORTD = com;
        if(com == 1){
            SSPBUF = lecADC;
        }
        else if(com == 0){
            SSPBUF = cont;
        }
        
        SSPIF = 0;
    }
    if (INTCONbits.RBIF){           // Revisa si hay interrupción del puerto B
        if (PORTBbits.RB7 == 0)     // Revisa si se presionó RB7 (cambio modo)
        {
            __delay_ms(20);
            cont = cont + 1;
        }
        if(PORTBbits.RB6 == 0){     // Revisa si se presionó RB6 (posición +1)
            __delay_ms(20);
            cont = cont - 1;
        }
        RBIF = 0;
    }
}
    
void main(void) {
    setupINTOSC(5);     // Oscilador a 2MHz
    portsetup();
    setup_portb();
    ADC_config(0x01);   // Configurar canal analógico 0
    cont = 0;
            
    while(1){
        lecADC = ADC_read(0);
        //PORTD = cont;
        __delay_ms(5);
    }
}

void portsetup(){
    TRISD = 0;
    PORTD = 0;
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
   
}

void setup_portb(void){
    TRISB = 0b11000000;
    INTCONbits.RBIE = 1;    // Habilita interrupción del puerto B
    INTCONbits.RBIF = 0;    // Apaga la bandera de interrupción del puerto B
    IOCB = 0b11000000;      // Habilita la interrupción en cambio (IoC)
    WPUB = 0b11000000;      // Habilita el Weak Pull-Up en el puerto B
    OPTION_REGbits.nRBPU = 0;   // Deshabilita el bit de RBPU
}