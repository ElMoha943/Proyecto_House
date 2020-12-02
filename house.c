#include<xc.h>
#include"house.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void SendSerial(unsigned char *data, unsigned char lenght)
{
    unsigned char x=0;
    while(x<lenght){
        while(PIR1bits.TXIF==0){}        
        TXREG=data[x];        
        x++;
    }            
}

void RX_Byte(void){
    unsigned char i;
    ArrayRX[Indice] = RCREG;
    while(PIR1bits.TXIF == 0){}
    TXREG = ArrayRX[Indice];
    if (ArrayRX[Indice] == 'e'){
        // ArrayRX a ArrayProc
        for (i=0;i<Indice;i++){
            ArrayProc[i] = ArrayRX[i];
        }
        ProcesarArray();
        Indice = 0;
    }
    else{
        Indice++;
    }
}

void ProcesarArray(void){
    unsigned char i, cosa=0;
    char Auxi[6];
    int Destino;
    //CONSULTA REF
    if ((ArrayProc[0]=='[')&&(ArrayProc[1]=='C')&&(ArrayProc[2]=='R')&&(ArrayProc[3]==',')&&(ArrayProc[5]==',')&&(ArrayProc[11]==']')){
        // En posiciones 6 a 10 tengo los caracteres que representan el número de dispositivo con quien se quiere comunicar 
        if(EsParaMi()){
            if(ArrayProc[4]=='1'){ // El maestro me solicita la REF1
                SendSerial(REF1,10);
            }
            if(ArrayProc[4]=='2'){ // El maestro me solicita la REF2
                SendSerial(REF2,50);
            }
        }
    }
    //PEDIDO DE ACCION
    else if ((ArrayProc[0]=='[')&&(ArrayProc[1]=='P')&&(ArrayProc[2]=='A')&&(ArrayProc[3]==',')&&(ArrayProc[5]==',')&&(ArrayProc[11]==',')&&(ArrayProc[14]==',')&&(ArrayProc[17]==']')){
        if(EsParaMi()){
            if(ArrayProc[4]=='1'){ // LEDS
                if(ArrayProc[12]=='1'){
                    if(LED1==1)LED1=0;
                    else LED1=1;
                }
                else if(ArrayProc[12]=='2')
                {
                    if(LED2==1)LED2=0;
                    else LED2=1;
                }
                else
                {
                    SendSerial("ERROR: debe ser 1/2",25);
                }
            }
            if(ArrayProc[4]=='2'){ // BUZZERS
               if(Buzzer==1)Buzzer=0;
               else Buzzer=1;
            }
            if(ArrayProc[4]=='3'){ // VENTILADOR
                if(ArrayProc[12]=='1'){
                    VentTrans1 = 0;
                    VentTrans2 = 0;
                }
                else if(ArrayProc[12]=='2')
                {
                    VentTrans1 = 1;
                    VentTrans2 = 0;
                }
                else if(ArrayProc[12]=='3')
                {
                    VentTrans1 = 0;
                    VentTrans2 = 1;
                }
                else
                {
                    SendSerial("ERROR: debe ser 1/2/3",25);
                }
            }
            if(ArrayProc[4]=='4'){ // RELEE
               if(Relay==1)Relay=0;
               else Relay=1;
            }
        }
    }
    //CONSULTA A DISPOSITIVO
    else if ((ArrayProc[0]=='[')&&(ArrayProc[1]=='C')&&(ArrayProc[2]=='O')&&(ArrayProc[3]==',')&&(ArrayProc[5]==',')&&(ArrayProc[11]==',')&&(ArrayProc[14]==',')&&(ArrayProc[17]==']')){
        unsigned char caca[6], valor=0;
        if(EsParaMi()){
            if(ArrayProc[4]=='1'){ //TEMPERATURA
                ADCON0bits.GO=1;
                while(ADCON0bits.GO==1)
                valor=(ADRESH<<8)+ADRESL;
                __delay_ms(1);
                caca[0] = (valor/100)+'0';
                caca[1] = ((valor/10)%10)+'0';
                caca[2] = ',';
                caca[3] = (valor%10)+'0';
                caca[4] = 'C';
                SendSerial(caca,6);
            }
        }
    }
    else SendSerial("sad",3);
}

unsigned char EsParaMi(){ //[CR,1,00001]e
    unsigned char Auxi[5];
        Auxi[0]=ArrayProc[6];
        Auxi[1]=ArrayProc[7];
        Auxi[2]=ArrayProc[8];
        Auxi[3]=ArrayProc[9];
        Auxi[4]=ArrayProc[10];        
        if(strcmp(Auxi,"00001")){
            return 1;
        }
        else {
            return 0;
        }
}