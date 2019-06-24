/*
 * File:   interrupt.h
 * Author: Coren
 *
 * Created on 16 mai 2019, 21:41
 */

#ifndef INTERRUPT_H
#define	INTERRUPT_H
#include "hardware.h"
#include "MI2C.h"

#define True 1
#define False 0

#define Rec_Telecom 0xA2

#define Haut		0b0010		// nouveau code de la touche du haut
#define Bas			0b0100		// nouveau code de la touche du bas
#define Gauche		0b0101		// nouveau code de la touche de gauche
#define Droite		0b0001		// nouveau code de la touche de droite
#define Centre		0b0011		// nouveau code de la touche centrale

extern int reloadTimer0;
extern char volatile flagINT0;
extern char flagSonar;
extern char cntTimer0;
extern char cntTimer2;
extern char delay;
extern volatile char update;

extern volatile char start;
extern volatile char flagBatterie;


extern volatile char buffTelecom;

/*extern unsigned char Send_I2C_Telecom_ASCII_Buf[8] = {0X31,0X00};
extern unsigned char Recv_I2C_Telecom_ASCII_Buf[8];*/


#endif	/* INTERRUPT_H */

