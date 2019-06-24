/*
 * File:   tools.c
 * Author: Coren
 *
 * Created on 16 mai 2019, 23:47
 */

#include <stdio.h>
#include <stdlib.h>

char abs(volatile char n){
    //printf("input abs %d",n);

    if(n<0)
        n = -n;
    //printf("    return %d\r\n",n);
    return n;
}

