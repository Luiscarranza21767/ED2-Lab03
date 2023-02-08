/* 
 * File:   conversiones.h
 * Author: Pablo
 *
 * Created on 27 de enero de 2023, 12:47 AM
 */

#ifndef CONVERSIONES_H
#define	CONVERSIONES_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

uint8_t descomponer(int pos, uint16_t num);
char inttochar(uint8_t num);
int chartoint(char num);
int convint(char centenas, char decenas, char unidades);
uint16_t mapeo(uint8_t valor, uint8_t inmin, uint8_t inmax, uint8_t outmin, uint16_t outmax);

#endif	/* CONVERSIONES_H */

