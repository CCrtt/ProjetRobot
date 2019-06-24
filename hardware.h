/*
 * File:   app.h
 * Author: Coren
 *
 * Created on 16 mai 2019, 23:35
 */

#ifndef APP_H
#define	APP_H
#include <stdio.h>
#include<p18f2520.h>
extern char delay;

extern void set_PWM_D(char val);
extern void set_PWM_G(char val);
extern char abs(char n);

void go_adc(void);
char get_adc(void);
#endif	/* APP_H */

