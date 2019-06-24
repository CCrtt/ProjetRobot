/*
 * File:   init.h
 * Author: Coren
 *
 * Created on 16 mai 2019, 21:38
 */

#ifndef INIT_H
#define	INIT_H

#include<p18f2520.h>
#include "MI2C.h"

#define True 1
#define False 0


extern int reloadTimer0;
extern char cntTimer0;
extern char cntTimer2;
extern char delay;


void init_robot();
#endif	/* INIT_H */

