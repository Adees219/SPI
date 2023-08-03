/* 
 * File:   slaveSpi.c
 * Author: USUARIO FINAL
 *
 * Created on 27 de julio de 2023, 11:23 AM
 */

//**********************************************************************************************
//palabra de configuracion
//**********************************************************************************************

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//**********************************************************************************************
//prototipos de funciones
//**********************************************************************************************
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config.h"
#include "ADC.h"
#include "SPI.h"

//**********************************************************************************************
//prototipo de funciones
//**********************************************************************************************
void setup(void);

//**********************************************************************************************
//variables
//**********************************************************************************************

#define _XTAL_FREQ 8000000
char tipo=0;
uint8_t contador=0;
//**********************************************************************************************
//ISR
//**********************************************************************************************
void __interrupt() isr(void)
{
       if(SSPIF == 1){
            tipo = spiRead();
            if(tipo==0){
            spiWrite(PORTD); 
            __delay_ms(1);
            }
            else{
            spiWrite(contador);
            __delay_ms(1); 
            }
         SSPIF = 0;  
       }
       
       if (ADIF){
           PORTD = adc_read();
           ADIF = 0;
       }
}


//**********************************************************************************************
//Codigo principal
//**********************************************************************************************
void main(void) {
    setup();

    while(1){
        
            if(ADCON0bits.GO == 0){
                __delay_ms(10);
                ADCON0bits.GO = 1; 
            }
        
        
            if(RB0 == 0){
                while(RB0 == 0);
                contador++;
            }
            else if(RB1 == 0){
                while(RB1 == 0);
                contador--;
            }       
        
       __delay_ms(250);
    }
    return;
}



//**********************************************************************************************
//Codigo de configuracion
//**********************************************************************************************
void setup(){
    
    configOsc(8); //configuracion del oscilador
    configPort(); //configuracion de puertos  
    configIntbits(); //configuracion de bits de interrupcion
    pullup(); //
    adc_init(0,8);
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    return;
}

/*******************************************************************/
//Funciones locales
/*******************************************************************/
