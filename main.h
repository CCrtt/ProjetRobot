/*
 * File:   main.h
 * Author: Coren
 *
 * Created on 3 mai 2019, 09:06
 */

#ifndef MAIN_H
#define	MAIN_H

#include <stdio.h>
#include <stdlib.h>


#include<p18f2520.h>
#pragma config OSC = INTIO67
#pragma config PBADEN = OFF, WDT = OFF, LVP = OFF, DEBUG = OFF

#include "init.h"
#include "interrupt.h"
#include "hardware.h"
#include "app.h"
#include "tools.h"
#include "MI2C.h"

#define True 1
#define False 0

volatile char start;
extern volatile char flagINT0;
extern volatile char flagBatterie;
extern volatile char update;
extern int mode;
extern volatile char buffTelecom;
extern char robotStart;
extern char flagSonar;
extern char flagUpdate;
unsigned int distSonar;

unsigned char tabUbatMoy[8];
unsigned char ubatMoy;


extern unsigned char moy(unsigned char tabMoy[8]);
extern void deplacement_robot(void);


void init_robot();


#endif	/* MAIN_H */

