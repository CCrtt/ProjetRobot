
/*
 * File:   interrupt.c
 * Author: Coren
 *
 * Created on 6 mai 2019, 11:17
 */

#include <stdio.h>
#include <stdlib.h>
#include <p18f2520.h>
#include "interrupt.h"


/*
 *
 */
void HighISR(void);
void LowISR(void);


/*
 *
 */
volatile char buffTelecom;
unsigned char Send_I2C_Telecom_ASCII_Buf[8] = {0X31,0X00};
unsigned char Recv_I2C_Telecom_ASCII_Buf[8];
unsigned char Valeur;
unsigned char AffI2C = 0x20;	// adresse de base (7 bits) pour l'affichage par PCF8574 normal
char x;



#pragma code HighVector=0x08
void IntHighVector(void)
{
    _asm goto HighISR _endasm
}
#pragma code

#pragma code LowVector=0x18
void InLowVector(void)
{
    _asm goto LowISR _endasm
}
#pragma code

/*
 *
 */

#pragma interrupt HighISR
void HighISR(void)
{
    if (INTCONbits.INT0IF){
        INTCONbits.INT0IF = 0;
        flagINT0 = True;
        start = True;
        Read_REC_Telecom(Rec_Telecom,Send_I2C_Telecom_ASCII_Buf,Recv_I2C_Telecom_ASCII_Buf);
        Valeur = Recv_I2C_Telecom_ASCII_Buf[1];							// masquage des 4 bits de poids faible
        //Octet_i2c (AffI2C, ecriture, Valeur &0x0F );	// affichage sur I2C (4bits faibles)
        printf("\nBouton telecommande %d\r\n",Valeur);

        x=0;
    }

    if(INTCONbits.TMR0IF){
        INTCONbits.TMR0IF = 0;
        TMR0H = reloadTimer0>>8;
        TMR0L = reloadTimer0;

        if(cntTimer0%3==0){ //TODO comme timer 2
            go_adc();
            cntTimer0 = 0;
        }
        else
             cntTimer0++;
    }

}

#pragma interrupt LowISR
void LowISR(void)
{
    if(PIR1bits.ADIF == 1){
        PIR1bits.ADIF = 0;
        flagBatterie = True;
    }

    if(PIR1bits.TMR2IF == 1){
        PIR1bits.TMR2IF = 0;
        x ++;
        if(cntTimer2){
            cntTimer2--;
        }
        else{   //toutes les 100ms
            cntTimer2 = 10;
            flagSonar = True;
        }
        if(delay)
            delay--;
    }
}
