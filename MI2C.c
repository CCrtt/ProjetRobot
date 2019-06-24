
/*******************************************************
*													   *
*				Gestionnaire I2C					   *
*													   *
*													   *
*													   *
*	Compilateur MCC18								   *
*	Auteur P. GENTRIC								   *
*	Version 1										   *
*******************************************************/

#include <p18f2520.h>	// Register definitions
#include <i2c.h>		// Librairie C18
#include "MI2C.h"

	// initialisation de la partie I2C
void MI2CInit(void)
{
	SSPSTAT = 0x80;				// Slew rate 100KHz
	SSPCON1 = 0x28;				// Master Mode Enable, Sclock = FOSC/(4 * (SSPADD + 1)) I2C bus,
	SSPCON2 = 0x00;
	SSPADD  = 0x13;				// Prediviseur pour SCL = 100KHz a 8MHz
}

/********************************************************************
*     Function Name:    Detecte_I2c                                 *
*     Return Value:     0 si detecte         							*
*     Parameters:       Adresse byte 				    			*
*     Description:      Detecte presence slave						*
* 																	*
********************************************************************/
 char Detecte_i2c(char adresse_i2c)
	{
  		while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // ensure module is idle
		SSPCON2bits.SEN=1;        // initiate START condition
		while(SSPCON2bits.SEN);	 // Attente fin start condition

  		SSPBUF = adresse_i2c;		// Envoi adresse
		while(SSPSTATbits.BF);	// Attente buffer TX vide
		while(SSPCON2bits.ACKSTAT);	//Attente ACK slave

		while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // ensure module is idle
    	SSPCON2bits.PEN=1;                  // send STOP condition
    	while ( SSPCON2bits.PEN );  // attente fin stop condition

		PIR1bits.SSPIF = 0;				//Remise à jour du flag pour IT Stop
  		return (0); 		// return with contents of SSPBUF
	}


//---------------------------------------------------------------------
// SONAR_Read()
// Reads an integer value from SONAR connected to the adresse_i2c bus at
// the specified location MSB[0X02]. with ranging inch ou cm ou us
// return an integer distance [MSB,LSB]
//---------------------------------------------------------------------
int SONAR_Read( char adresse_i2c, char registre_distance)
{
union
{
char b[2];
int i;
}
data;

union
{
char b[2];
int i;
}
temp;

char error,retry;
error = 0;

retry = 10;								// Poll the SONAR up to 10 times MAX
do
	{
	error = EEAckPolling(adresse_i2c);
	retry--;
	} while(error && retry > 0);

retry = 10;								// Attempt to read high byte of data
do											// up to 10 times MAX
	{
	temp.i = EERandomRead(adresse_i2c, registre_distance);
	retry--;
	} while(temp.b[1] && retry > 0);

if(temp.b[1]) data.b[0] = 0xFF;		// Make read result 0 if error
else data.b[1] = temp.b[0];		// Otherwise get the high byte of data

retry = 10;								// Attempt to read low byte of data
do											// up to 10 times MAX
	{
	temp.i = EERandomRead(adresse_i2c, registre_distance + 1);
	retry--;
	} while(temp.b[1] && retry > 0);

if(temp.b[1]) data.b[1] = 0xFF;		// Make read result 0 if error
else data.b[0] = temp.b[0];		// Otherwise get the low byte of data

return data.i;
}
//---------------------------------------------------------------------
// SONAR_Write()
// Writes a char commande value to SONAR connected to the adresse_i2c bus at
// the specified location 0x00 with commande ranging inch, us, or cm
//---------------------------------------------------------------------
void SONAR_Write( char adresse_i2c, char commande)
{
char error, retry;
error = 0;

retry = 10;								// Poll the SONAR up to 10 times MAX
do
	{
	error = EEAckPolling(adresse_i2c);
	retry--;
	} while(error && retry > 0);

retry = 10;								// Attempt to write low byte of data
do											// up to 10 times MAX
	{
	error = EEByteWrite(adresse_i2c, 0x00, commande);
	retry--;
	} while(error && retry > 0);

}

/********************************************************************
*     Function Name:    Ecrire_i2c_Telecom                                	*
*     Return Value:     0 si OK         							*
*     Parameters:       Adresse byte, Pointeur sur octet a ecrire 	*
*     Description:      Ecrire un buffer formaté string				*
* 																	*
********************************************************************/

	char Ecrire_i2c_Telecom(char adresse_i2c, char* Send_Buff)
	{
		char index = 0;
	  	while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // ensure module is idle
		SSPCON2bits.SEN=1;        // initiate START condition
		while(SSPCON2bits.SEN);	 // Attente fin start condition

  		SSPBUF = adresse_i2c;		// Envoi adresse
		while(SSPSTATbits.BF);	// Attente buffer TX vide
		while(SSPCON2bits.ACKSTAT);	//Attente ACK slave

		do
		{
			while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // ensure module is idle
	 		SSPBUF = Send_Buff[index];		// Ecrit octet
	 		while(SSPSTATbits.BF);	// Attente buffer TX vide
	 		while(SSPCON2bits.ACKSTAT);	//Attente ACK slave
	 	}while(Send_Buff[index++]);

		while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // ensure module is idle
    	SSPCON2bits.PEN=1;                  // send STOP condition
    	while ( SSPCON2bits.PEN );  // attente fin stop condition

		PIR1bits.SSPIF = 0;				//Remise à jour du flag pour IT Stop
  		return (0); 		// return with contents of SSPBUF
	}

/********************************************************************
*     Function Name:    Lire_i2c_Telecom                                	*
*     Return Value:     0 si OK         							*
*     Parameters:       Adresse byte, Pointeur sur octet lu 		*
*     Description:      Lire un buffer formaté string				*
* 																	*
********************************************************************/

	char Lire_i2c_Telecom(char adresse_i2c, char* Recv_Buff)
	{
		char index = 0;
	  	while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // ensure module is idle
		SSPCON2bits.SEN=1;        // initiate START condition
		while(SSPCON2bits.SEN);	 // Attente fin start condition

  		SSPBUF = adresse_i2c +1;		// Envoi adresse
		while(SSPSTATbits.BF);	// Attente buffer TX vide
		while(SSPCON2bits.ACKSTAT);	//Attente ACK slave


		do
		{
	 		while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // ensure module is idle

	 		SSPCON2bits.RCEN=1;        // SSPBUF en reception
	 		while(!SSPSTATbits.BF);	// Attente buffer RX plein
	 		Recv_Buff[index] = SSPBUF;		// Ecrit octet
	 		if(SSPBUF)
	 			SSPCON2bits.ACKDT=0;        // initiate ACK
	 		else
	 			SSPCON2bits.ACKDT=1;        // initiate NoACK
	 		SSPCON2bits.ACKEN=1;
	 		while(SSPCON2bits.ACKEN);	// Attendre fin ACKEN
	 	}while(Recv_Buff[index++]);	//Tant que not \'0'

		while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // ensure module is idle
    	SSPCON2bits.PEN=1;                  // send STOP condition
    	while ( SSPCON2bits.PEN );  // attente fin stop condition

		PIR1bits.SSPIF = 0;				//Remise à jour du flag pour IT Stop
  		return (0); 		// return with contents of SSPBUF
	}


/********************************************************************
*     Function Name:    Write_PCF8574                               *
*     Return Value:     erreur 0 si OK         						*
*     Parameters:       Adresse_i2c byte, data byte 				*
*     Description:      Write data at current address of chip 8574  *
*                       device.										*
*                                                                   *
********************************************************************/
char Write_PCF8574(char adresse_i2c, char data )
{
	char error = 0;
  	while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // ensure module is idle
	SSPCON2bits.SEN=1;        // initiate START condition
	while(SSPCON2bits.SEN);	 // Attente fin start condition
  	SSPBUF = adresse_i2c;		// Write adresse mode Ecriture un octet
	while(SSPSTATbits.BF);	// Attente buffer TX vide
	while(SSPCON2bits.ACKSTAT);	//Attente ACK slave

  	while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // ensure module is idle
	SSPBUF = data;				//Ecrit data pour transmission
	while(SSPSTATbits.BF);			// Attente buffer TX vide
	while(SSPCON2bits.ACKSTAT);	//Attente ACK slave

  	while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // ensure module is idle

    SSPCON2bits.PEN=1;                  // send STOP condition
    while ( SSPCON2bits.PEN );  // attente fin stop condition
	PIR1bits.SSPIF = 0;				//Remise à jour du flag pour IT Stop
  	return (error); 				// return with  error
}

/********************************************************************************************
*     Function Name:    Read_REC_Telecom                                                    *
*     Return Value:     signed char -1 si erreur 0 si OK         	                        *
*     Parameters:       char control Adresse physique sur bus I2C (R/W = 0)                 *
*						*ptr_send poiteur sur buffer mode de fonctionnement interface *
*							chaine de deux char '1','\0' mode normal reception		        *
*							chaine de deux char '2','\0' mode test							*
*						*ptr_recv pointeur sur buffer chaine char recus               *
*							termine par '\0'									            *										  *
*     Description:      Effectue la lecture des donnees recues dans le mode                 *
*                       specifique										                    *
*                                                                                           *
*********************************************************************************************/
signed char Read_REC_Telecom(unsigned char control,char *ptr_send,char *ptr_recv)
	{
	char error = 0;
	char temp;
  	while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // module disponible?
	SSPCON2bits.SEN=1;        // initialise START condition
	while(SSPCON2bits.SEN);	 // Attente fin start condition
	temp = SSPBUF;			// Vide SSPBUF
	do
		{
	  	SSPBUF = (control|0x00);		// Ecrit adresse physique R/nW = 0
		if(SSPCON1bits.WCOL)	// Si collision
			error = -1;			// signale erreur collision
		while(SSPSTATbits.BF);	// Attente buffer TX vide
	  	while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // module disponible?
		if(SSPCON2bits.ACKSTAT)	//si nACK esclave
			error = -2;			// signale l'erreur
		else
			error = 0;			// Pas d'erreur
		if(error == -1)	// Si collision
			{
			temp = SSPBUF;	// on vide SSPBUF
			SSPCON1bits.WCOL = 0;	// reset collision bit WCOL
			}
		}while(error !=0);		// Fin envoie adresse physiqu

	do						// Ecriture du Buffer mode de fonctionnement
		{
		do
			{
	  		SSPBUF = *ptr_send;		// Transmission du mode de fonctionnement
			*ptr_send++;
			if(SSPCON1bits.WCOL)	// Si collision
				error = -1;
			while(SSPSTATbits.BF);	// Attente buffer TX vide
	  		while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // module disponible ?
			if(SSPCON2bits.ACKSTAT)	//si nACK esclave
				{
				error = -2;			// signale erreur
 				while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // module disponible ?
				SSPCON2bits.RSEN=1;        // initialise RE-START condition
				while(SSPCON2bits.RSEN);	 // Attente fin RE_START condition
				temp = SSPBUF;	// Pour vider SSPBUF
				do
					{
	  				SSPBUF = (control|0x00);		// Ecrit adresse physique R/nW = 0
					if(SSPCON1bits.WCOL)	// Si collision
						error = -1;			// signale erreur collision
					while(SSPSTATbits.BF);	// Attente buffer TX vide
				  	while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // module disponible?
					if(SSPCON2bits.ACKSTAT)	//si nACK esclave
						error = -2;			// signale l'erreur
					else
						error = 0;			// Pas d'erreur
					if(error == -1)	// Si collision
						{
						temp = SSPBUF;	// on vide SSPBUF
						SSPCON1bits.WCOL = 0;	// reset collision bit WCOL
						}
					}while(error !=0);		// Fin envoie adresse physiqu
							}
						else
							error = 0;			// pas d'erreur
						if(error == -1)	// Si collision
							{
							temp = SSPBUF;	// on vide SSPBUF
							SSPCON1bits.WCOL = 0;	// reset collision bit WCOL
							}
						}while(error !=0);
					}while(SSPBUF != '\0');		// Fin envoie Buffer


  	while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // module disponible ?
	SSPCON2bits.RSEN=1;        // initialise RE-START condition
	while(SSPCON2bits.RSEN);	 // Attente fin RE_START condition
	temp = SSPBUF;	// Pour vider SSPBUF
	do
		{
  		SSPBUF = (control|0x01);		// Ecrit adresse physique R/nW = 1
		if(SSPCON1bits.WCOL)	// Si collision
			error = -1;			// signale erreur
		while(SSPSTATbits.BF);	// Attente buffer TX vide
  		while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));  // module disponible ?
		if(SSPCON2bits.ACKSTAT)	//si nACK esclave
			error = -2;			// signale erreur
		else
			error = 0;			// pas d'erreur
		if(error == -1)			// Si collision
			{
			temp = SSPBUF;	// on vide SSPBUF
			SSPCON1bits.WCOL = 0;	// reset collision bit WCOL
			}
		}while(error !=0);
	do							// Boucle de reception des octets
		{
		SSPCON2bits.RCEN = 1;    	//Commute Maitre I2C en reception pour tous les octets
		while(!SSPSTATbits.BF);		// Attente buffer TX plein
  		*ptr_recv = SSPBUF;	// Archive dans le buffer Recv
		while(SSPCON2bits.RCEN);	// Attente fin RCEN ms ? 0 par Hardware
		*ptr_recv++;
			if(SSPBUF != '\0')		// si pas fin reception ?
				{
				SSPCON2bits.ACKDT = 0;	// ACK pour transaction
				SSPCON2bits.ACKEN = 1;	// Envoi ACK
				while(SSPCON2bits.ACKEN);	// Attendre fin ACK
				}
			else					// si dernier octet recu ?
				{
				SSPCON2bits.ACKDT = 1;	// nACK pour transaction
				SSPCON2bits.ACKEN = 1; 	// Envoi nACK fin transaction
				while(SSPCON2bits.ACKEN);	// Attendre fin nACK
				}
		}while (SSPBUF != '\0');	// Fin reception
	SSPCON2bits.PEN = 1;            // envoi STOP condition
    while(SSPCON2bits.PEN);  		// attente fin stop condition
  	return (error); 				// retour ode de l'erreur
	}


unsigned char Octet_i2c(unsigned char adresse, unsigned char sens, unsigned char valeur)
// dialogue avec le destinataire d?fini par adresse
// si SENS = 1 : on lit ? l'adresse et on rend la donn?e en valeur
// si SENS = 0 : on ?crit l'octet valeur obtenu mais compl?ment? car les Leds sont tir?es au NLH
// On travaille en maitre avec un diviseur d'horloge pour un bit de excatement 10?s soit F = 40kHz
{
	unsigned char t;			// tampon interm?diaire pour octet re?u et pour compte-rendu
	if (sens == lecture)
	{
		while((SSPCON2 & 0x1F) || SSPSTATbits.R_W);  // Attente module idle
 		SSPCON2bits.SEN = 1;			// START
 		while (SSPCON2bits.SEN);		// Attente de fin de condition start (RAZ automatique)
 		SSPBUF = (adresse << 1) + sens; 	// Ecriture de l'adresse avec le bit Lecture
		while (SSPSTATbits.BF);			// on attend la fin d'?criture de l'adresse
		while (SSPCON2bits.ACKSTAT);		// on attend que l'acquittement ait eu le temps d'?tre mis

		while( SSPCON2 & 0x1F | SSPSTATbits.R_W)
 		SSPCON2bits.RCEN = 1;			// On passe en mode r?ception
		while (!SSPSTATbits.BF);		// On attend la fin de r?ception de l'octet mis par le destinataire
		t = SSPBUF;			// Lecture de l'octet re?u
 		SSPCON2bits.ACKDT = 1;			// pas d'acquittement car on arr?te l? : NON-ACK
 		SSPCON2bits.ACKEN = 1; 			// lancement du cycle acquittement
 		while (SSPCON2bits.ACKEN);		// dont on attend la fin

 		SSPCON2bits.PEN = 1;			// lancement de la condition de STOP
 		while (SSPCON2bits.PEN);		// On attend la fin de temps de stop
		PIR1bits.SSPIF = 0;			// effacement du drapeau qui indiquera la fin de stop
 		return (t); 		// t ?tant l'octet re?u, on le retourne
	}
	if (sens == ecriture)
	{
		while((SSPCON2 & 0x1F) || SSPSTATbits.R_W);  // Attente module idle
 		SSPCON2bits.SEN = 1;				// START
 		while (SSPCON2bits.SEN);			// Attente de fin de condition start (RAZ automatique)
 		SSPBUF = (adresse << 1) + sens; 	// Ecriture de l'adresse avec le bit Ecriture
		while (SSPSTATbits.BF);			// Attendre fin ecriture adresse
		while(SSPCON2bits.ACKSTAT);		// Attendre ACK slave

		while((SSPCON2 & 0x1F) || SSPSTATbits.R_W);  // Attente module idle
		SSPBUF = ~valeur;	// on ?crit l'octet ? ?mettre en compl?ment? et c'est parti
		while(SSPSTATbits.BF);			// Attendre buffer vide
		while (SSPCON2bits.ACKSTAT);	// on attend  ACK

		while((SSPCON2 & 0x1F) || SSPSTATbits.R_W);  // Attente module idle
 		SSPCON2bits.PEN = 1;				// STOP
 		while (SSPCON2bits.PEN);			// attente fin du STOP
		PIR1bits.SSPIF =0;				//Remise ? jour du flag pour IT Stop
	}
}

//---------------------------------------------------------------------
// SONAR_Ecrit()
// Ecrit un octet de commande dans le sonar connect? ? l'I2C
// ? l'adresse indiqu?e 0x00.
//---------------------------------------------------------------------

void SONAR_Ecrit (char controle, unsigned char adresse, char data)
{
	// variables locales
	char erreur, essai;

	erreur = 0;
	essai = 10;						// On essaiera 10 fois d'acc?der au SONAR
	do
	{
		erreur = EEAckPolling (controle);
		essai--;
	}
	while (erreur && essai > 0);
	essai = 10;						// R?essai 10 fois MAX
	do								// Tentative d'?criture de l'octet faible de donn?e
	{
		erreur = EEByteWrite (controle, adresse, data);
		essai--;
	}
	while (erreur && essai > 0);
}

//---------------------------------------------------------------------
// SONAR_Lit()
// Reads an integer value from SONAR connected to the I2C bus at
// the specified location MSB[0X02], LSB[0x03].
// Return 0x0000 if error or distance = 0x[MSB][LSB] ranging
//---------------------------------------------------------------------

unsigned int SONAR_Lit (char controle, unsigned char adresse)
{
// variables locales
	union
		{	char b[2];
			int i;
		}	data;
	union
		{	char b[2];
			int i;
		}	temp;
	char erreur, essai;

	erreur = 0;
	essai = 10;							// On essaiera 10 fois d'acc?der au SONAR
	do
	{
		erreur = EEAckPolling (controle);
		essai--;
	}
	while (erreur && essai > 0);
	essai = 10;							// R?essai 10 fois MAX
	do									// Tentative de lecture de l'octet fort de donn?e
	{
		temp.i = EERandomRead (controle, adresse);
		essai--;
	}
	while (temp.b[1] && essai > 0);
	if (temp.b[1]) data.b[0] = 0xFF;	// Mise du r?sultat de lecture ? 0 si erreur
	else data.b[1] = temp.b[0];			// Sinon, mise de l'octet fort de donn?e lue
	essai = 10;							// R?essai 10 fois MAX
	do									// Tentative de lecture de l'octet faible de donn?e
	{
		temp.i = EERandomRead (controle, adresse + 1);
		essai--;
	}
	while (temp.b[1] && essai > 0);
	if (temp.b[1]) data.b[1] = 0xFF;	// Mise du r?sultat de lecture ? 0 si erreur
	else data.b[0] = temp.b[0];			// Sinon, mise de l'octet faible de donn?e lue
	return data.i;
}