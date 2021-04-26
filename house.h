#ifndef HOUSE_H
#define	HOUSE_H

#define _XTAL_FREQ 4000000
#define LED1 RC0
#define LED2 RC1
#define Relay RC2
#define Buzzer RC3
#define VentTrans1 RC4
#define VentTrans2 RC5

//VARIABLES
unsigned char J[5]="00001", REF1[10]="Dir1", REF2[50]="Controla los aparatos de una casa inteligente.";
unsigned char recibido, comando, command_data[10], command_data_index=0, command;
unsigned char ArrayRX[20], Indice, ArrayProc[20];

//FUNCIONES
void SendSerial(unsigned char *data);
void RX_Byte(void);
void ProcesarArray(void);
unsigned char EsParaMi(void);

#endif	/* HOUSE_H */
