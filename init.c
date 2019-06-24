/*
 * File:   init.c
 * Author: Coren
 *
 * Created on 3 mai 2019, 09:08
 */

#include "init.h"


void init_robot(void);
volatile char flagINT0;
char robotStart;
volatile char flagBatterie;
char flagSonar;
char flagUpdate;
int reloadTimer0;
char cntTimer0; 
char cntTimer2;
/*
 *
 */
void init_robot(){
    /********************INIT CLOCK*******************/
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;


    /*********************PWM*************************/
    PR2 = 124; // PWM period 1ms
    CCPR1L = 0; //duty cycle 0
    CCPR2L = 0; //duty cycle 0

    T2CONbits.T2OUTPS0 = 1; //1:10 postcaler
    T2CONbits.T2OUTPS1 = 0;
    T2CONbits.T2OUTPS2 = 0;
    T2CONbits.T2OUTPS3 = 1;

    T2CONbits.T2CKPS0 = 0;  //1:16 prescaler
    T2CONbits.T2CKPS1 = 1;

    cntTimer2 = 10;         //periode de 100ms pour le sonar
    IPR1bits.TMR2IP = 0;
    PIE1bits.TMR2IE = 1;    //interrupt on timer2 every 10ms
    T2CONbits.TMR2ON = 1;   //timer2 on

    CCP1CON = 0b00001100;   //mode PWM
    CCP2CON = 0b00001100;   //mode PWM

    TRISCbits.RC1=0; //PWM droit
    TRISCbits.RC2=0; //PWM gauche
    TRISAbits.RA6=1; //dir droite
    TRISAbits.RA7=1; //dir gauche

    /*****************ADC******************************/
   ADCON1bits.PCFG0 = 0; // permet Analog 0, 1 et 2
   ADCON1bits.PCFG1 = 0;
   ADCON1bits.PCFG2 = 1;
   ADCON1bits.PCFG3 = 1;

   ADCON1bits.VCFG0 = 0; //(sélection Vref et - Vref sur Vss et Vdd)
   ADCON1bits.VCFG1 = 0;

   ADCON2bits.ADCS0 = 1; //(conversion time de Fosc/4)
   ADCON2bits.ADCS1 = 0;
   ADCON2bits.ADCS2 = 0;

   ADCON2bits.ACQT0 = 0; //(acquisition time de 4 Tad)
   ADCON2bits.ACQT1 = 1;
   ADCON2bits.ACQT2 = 1;
   //ADCON2 = 0x14;  //justif à gauche, adclk 1us, Tacq 4us

   TRISAbits.RA0=1;
   TRISAbits.RA1=1;
   TRISAbits.RA2=1;//Vbatt
   ADCON0bits.CHS = 2; //(sélection channel 2, AN2)

   ADCON2bits.ADFM = 0;//justifie a gauche

   ADCON0bits.ADON = 1; //(enable converter)

   IPR1bits.ADIP = 0;
   PIR1bits.ADIF = 0;
   PIE1bits.ADIE = 1; // configurer l'interruption de l'adc set ADIE


   /************************INT0************************/
   INTCON2bits.INTEDG0 = 0;
   INTCONbits.INT0IE = 1;
   flagINT0 = False;


   /**********************TIMER0**************************/
   T0CONbits.T08BIT = 0;
   T0CONbits.T0CS = 0;
   T0CONbits.PSA = 0;
   T0CONbits.T0PS = 100;    //prescaler 32
   INTCONbits.TMR0IE = 1;
   INTCONbits.TMR0IF = 1;

   reloadTimer0 = 3036;
   cntTimer0 = 0;

   T0CONbits.TMR0ON = 1;

   /**************************UART**************************/
    //Baud rate formula [Fosc/4(n+1)]
	BAUDCONbits.BRG16 = 1; // 8 bits
	TXSTAbits.BRGH = 1;
	TXSTAbits.SYNC = 0; // asynchrone
	SPBRGH = 0;
	SPBRG = 207;

	TRISCbits.RC6 = 1; //RX
    TRISCbits.RC7 = 1; //TX

	RCSTAbits.SPEN = 1; //asynchrone clear SYNC + set SPEN to 1
	TXSTAbits.TXEN = 1; // enable transmission, set TXIF


	PIE1bits.TXIE = 0;
	PIR1bits.TXIF = 0;

    /***************I2C***********************/
    /*SSPSTAT = 0x80;				// Slew rate 100KHz
	SSPCON1 = 0x28;				// Master Mode Enable, Sclock = FOSC/(4 * (SSPADD + 1)) I2C bus,
	SSPCON2 = 0x00;
	SSPADD  = 0x13;	*/			// Prediviseur pour SCL = 100KHz a 8MHz
    MI2CInit();

    TRISCbits.RC3=1; //SCL
    TRISCbits.RC4=1; //SDA

    /***********************AUTRES ENTREES/SORTIES**************************/

    TRISAbits.RA6 = 0; //Dir D
    TRISAbits.RA7 = 0; //Dir G
    TRISBbits.RB0=1; //bouton télécommande
    TRISBbits.RB5=0; //led



    /****************INIT VARIABLES GLOBALES***********/
    robotStart = 0;
    flagBatterie = 0;
    flagSonar = 0;
    flagUpdate = 0;
    delay = 0;

    /*****************ENABLE ALL INTERRUPT**************************************/
    RCONbits.IPEN = 1;   //enable priority
    INTCONbits.GIEL = 1; //enable low priority interrupt (IPEN = 1)
    INTCONbits.GIEH = 1;  //global interrupt enable
}

