#include<xc.h>
#include"house.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void SendSerial(unsigned char *data)
{
    unsigned char x=0;
    while(data[x]!='\0'){
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
    if((ArrayProc[6]==RD0+'0')&&(ArrayProc[7]==RD1+'0')&&(ArrayProc[8]==RD2+'0')&&(ArrayProc[9]==RD3+'0')&&(ArrayProc[10]==RD4+'0')){
    //CONSULTA REF
        if ((ArrayProc[0]=='[')&&(ArrayProc[1]=='C')&&(ArrayProc[2]=='R')&&(ArrayProc[3]==',')&&(ArrayProc[5]==',')&&(ArrayProc[11]==']')){
            // En posiciones 6 a 10 tengo los caracteres que representan el número de dispositivo con quien se quiere comunicar 
            if(ArrayProc[4]=='1'){ // El maestro me solicita la REF1
                SendSerial(REF1);
            }
            if(ArrayProc[4]=='2'){ // El maestro me solicita la REF2
                SendSerial(REF2);
            }
        }
        //PEDIDO DE ACCION
        else if ((ArrayProc[0]=='[')&&(ArrayProc[1]=='P')&&(ArrayProc[2]=='A')&&(ArrayProc[3]==',')&&(ArrayProc[5]==',')&&(ArrayProc[11]==',')&&(ArrayProc[14]==',')&&(ArrayProc[17]==']')){
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
                    SendSerial("ERROR: debe ser 1/2");
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
                    SendSerial("ERROR: debe ser 1/2/3");
                }
            }
            if(ArrayProc[4]=='4'){ // RELEE
               if(Relay==1)Relay=0;
               else Relay=1;
            }
        }
        //CONSULTA A DISPOSITIVO
        else if ((ArrayProc[0]=='[')&&(ArrayProc[1]=='C')&&(ArrayProc[2]=='O')&&(ArrayProc[3]==',')&&(ArrayProc[5]==',')&&(ArrayProc[11]==',')&&(ArrayProc[14]==',')&&(ArrayProc[17]==']')){
            if(ArrayProc[4]=='1'){ //TEMPERATURA
                unsigned int caca;
                unsigned char cosa[4];
                ADCON0bits.GO=1;
                while (ADCON0bits.GO==1)
                caca= (ADRESH<<8)+ADRESL;
                __delay_ms(2);
                valor= (((float)caca)*5/1023)*100;
                sprintf(cosa,"%.1f",valor);
                SendSerial(cosa)
            }
        }
    }
}
