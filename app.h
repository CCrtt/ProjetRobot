/*
 * File:   app.h
 * Author: Coren
 *
 * Created on 17 mai 2019, 00:14
 */

#ifndef APP_H
#define	APP_H
#include <stdio.h>
#include<p18f2520.h>
#include "hardware.h"

/*void set_PWM_D(char val);
void set_PWM_G(char val);
char abs(char n);*/

#define True 1
#define False 0

volatile char update;
char mode;
char delay;

extern void deplacement_robot(void);
extern void set_PWM_D(char val);
extern void set_PWM_G(char val);
extern unsigned char moy(unsigned char tabMoy[4]);


#endif	/* APP_H */

