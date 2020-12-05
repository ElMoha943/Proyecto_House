// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = ON        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include "house.h"
#include <string.h>

void main(){
    //Configura entradas y salidas
    ANSEL=0;
    ANSELH=0;
    OPTION_REG=0x04;
    TRISB=0x3F;
    TRISA=0xFF;
    TRISC0=0;
    TRISC1=0; // Configuro el pin CCP como salida en el puerto C
    TRISC2=0;
    TRISC3=0;
    TRISC4=0;
    TRISC5=0;
    TRISB5=0;
    TRISB6=0;
    
    //Configura UART a 9600 baudios
    TXSTAbits.TXEN=1;
    TXSTAbits.BRGH=1;
    RCSTAbits.SPEN=1;
    RCSTAbits.CREN=1;
    BAUDCTLbits.BRG16=0;
    SPBRG=25;
    SPBRGH=0;
    OPTION_REG=0x04;
    
    //Configura puerto AD
    ANSELH=0;
    ANSELHbits.ANS9=1; 
    ADCON1bits.ADFM=1;
    ADCON1bits.VCFG0=0; ADCON1bits.VCFG1=0;
    ADCON0bits.ADON=1;
    ADCON0bits.CHS0=1; ADCON0bits.CHS1=0; ADCON0bits.CHS2=0; ADCON0bits.CHS3=1;
    
    //Configura PWM
    CCP1CON = 0; // Apago módulo CCP
    TMR2 = 0; // Limpio el Timer2
    PR2 = 0x7F; // Cargo el periodo del PWM
    CCPR1L = 0x1F; // Cargo el duty cycle
    INTCON = 0; // Deshabilito interrupciones y limpio T0IF
    CCP1CON = 0x0C; // Configuro modo PWM
    // 2 bits menos significativos del Duty cycle = 00
    T2CONbits.TOUTPS0 = 1;
    T2CONbits.TOUTPS1 = 0;
    T2CONbits.TOUTPS2 = 0;
    T2CONbits.TOUTPS3 = 0;
    T2CONbits.TMR2ON = 1; // Arranco Timer2
    
    LED1 = 0;
    LED2 = 0;
    Relay = 0;
    Buzzer = 0;
    VentTrans1 = 0;
    VentTrans2 = 0;
    while(1){
        if (RCIF == 1){
            RX_Byte();
        }
//        if (PIR1bits.TMR2IF == 1){
//            if (CCPR1L == 0x1F){
//                CCPR1L = 0x3F;
//            }
//            else{
//                CCPR1L = 0x1F;
//            }
//            PIR1bits.TMR2IF = 0;
//        }
    }
}