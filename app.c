/*
 * File:   app.c
 * Author: Coren
 *
 * Created on 17 mai 2019, 00:14
 */

#include "app.h"

/*
 *
 */

unsigned char moy(unsigned char tabMoy[4]);
void deplacement_robot(void);

void deplacement_robot(void){
    update = False;

    switch(mode){
        case 0:
            set_PWM_G(0);
            set_PWM_D(0);
            break;

        case 1:
            set_PWM_G(30);
            set_PWM_D(30);

            break;

        case 2:
            set_PWM_G(75);
            set_PWM_D(85);
            break;
    }
}


unsigned char moy(unsigned char tabMoy[8]){
    unsigned int moy = 0;
    char i;

    for(i=0;i<8;i++){
        moy+=tabMoy[i];
    }
    moy = moy>>3;
    return (unsigned char)moy;
}
