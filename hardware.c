/*
 * File:   app.c
 * Author: Coren
 *
 * Created on 16 mai 2019, 23:38
 */
#include "hardware.h"

/*
 *
 */

void set_PWM_D(char val){
    int cmdD;
    //printf("valD %d     ",val);
    if(abs(val)<100){    //duty cycle <50%
        cmdD = abs(val);
     //printf("abs val D = %d",cmdD);
    }
    else
        cmdD = 100;



    if(val>0)
        PORTAbits.RA6 = 1;
    else
        PORTAbits.RA6 = 0;


    if(val ==0)
        CCPR2L = 0;
    else
        if (val>60)
             CCPR2L = (cmdD * 62)/100  -   11; //correction tout droit
        else
            CCPR2L = (cmdD * 62)/100;
}

void set_PWM_G(char val){
    int cmdG;
    //printf("valG %d\r\n",val);
    if(abs(val)<101)    //duty cycle <50%
        cmdG = abs(val);
    else
        cmdG = 100;

    if(val>0)
        PORTAbits.RA7 = 1;
    else
        PORTAbits.RA7 = 0;


    if(val == 0)
        CCPR1L = 0;
    else
      CCPR1L = (cmdG * 62)/100;

}

void go_adc(void){
    ADCON0bits.GO = 1;
}

char get_adc(void){
    return ADRESH;
}
