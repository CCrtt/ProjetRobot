
/*
 * File:   main.c
 * Author: Coren
 *
 * Created on 3 mai 2019, 08:49
 */
#include"main.h"


/*
 *
 */


void main(void) {
    unsigned char indiceBat = 0;
    unsigned char readyMoy = False;

    mode = -1;

    init_robot();

    tabUbatMoy[0] = 150;
    tabUbatMoy[1] = 150;
    tabUbatMoy[2] = 150;
    tabUbatMoy[3] = 150;
    tabUbatMoy[4] = 150;
    tabUbatMoy[5] = 150;
    tabUbatMoy[6] = 150;
    tabUbatMoy[7] = 150;

    start = False;
    printf("\nProjet Robot Vincent Corentin\n\r");
    while(1){
        Octet_i2c(0x20, 0, mode);
        while(start){
         Octet_i2c(0x20, 0, mode);
           if(flagBatterie){
               unsigned char ubat;
               flagBatterie = False;

               ubat = get_adc();
               tabUbatMoy[indiceBat] = ubat;
               //printf("bat %d,%d V\n\r",(int) ((float) ((int) ubatMoy) * 0.0727), (int) ((float) ((int) ubatMoy) * 7.84 - (int) (ubatMoy) * 7.84));
               printf("bat %d mV\n\r",(int) ((float) ((int) ubatMoy) * 72.7));
               if(readyMoy){
                   ubatMoy = moy(tabUbatMoy);
               }
               else
                   ubatMoy = ubat;

               if(ubatMoy<145){
                   start = False;
                   printf("\nBatterie faible\n\r");

               }

               if(indiceBat==0 && indiceBat%8==0){
                   indiceBat = 0;
                   readyMoy = True;
               }
               else{
                  indiceBat++;
               }
           }

           if(update){
               deplacement_robot();
           }

           if(flagSonar){
               flagSonar = False;

               distSonar = SONAR_Lit (0xE0,0x02);   //distance en cm lue par le sonar (adresse I2c, adresse registre)
               printf("distance : %d cm ",distSonar);
               SONAR_Ecrit(0xE0,0x00,0x51);   //demande de mesure (adresse I2c, adresse registre commande,commande pour avoir mesure cm)

               //analyse de la distance
               if(distSonar<40){
                   printf("distance detectee : <40. arret\r\n",mode);
                   if(mode != 0){
                     mode = 0;
                     update = True;
                   }
               }
                else if(distSonar>150){
                    printf("distance detectee >150 lent\r\n",mode);
                    if (mode != 1){
                        mode = 1;
                        update = True;
                    }
                }
                else if(distSonar>50){
                    printf("distance detectee >50 fonce sur cible\r\n",mode);
                    if(mode != 2){
                        mode = 2;
                        update = True;
                    }
                }
            }

            if (start==False)
            {
                mode = 0;
                deplacement_robot();
            }
        }
    }
}


