/* 
 * File:   mSPI.c
 * Author: USUARIO FINAL
 *
 * Created on 27 de julio de 2023, 10:42 AM
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
#include "SPI.h"
#include "LCD.h"

//**********************************************************************************************
//prototipo de funciones
//**********************************************************************************************
void setup(void);
void escrituraLCD(void);
//**********************************************************************************************
//variables
//**********************************************************************************************

#define _XTAL_FREQ 8000000
char ResSlave1[5];
char ResSlave2[5];
char ResSlave2C[5];
char temp1;
char temp2;
char temp3;
char tipo = 0;
//**********************************************************************************************
//ISR
//**********************************************************************************************
void __interrupt() isr(void)
{

}


//**********************************************************************************************
//Codigo principal
//**********************************************************************************************
void main(void) {
    setup();

    while(1){

        if(RB0 == 0){
            while(RB0 == 0);
            
            if(tipo == 0)
                tipo=1;
            else
                tipo=0;
        }
        
        
        
    /************** Slave 1**********************/
             
       PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(tipo);
       temp1 = spiRead();
       
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave Deselect 
       
        
    /************** Slave 2 adc**********************/
    if(tipo == 0){   
       PORTCbits.RC1 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(tipo);
       temp2 = spiRead();
       
       __delay_ms(1);
       PORTCbits.RC1 = 1;       //Slave Deselect 
    }
       
    /************** Slave 2 cont**********************/
    if(tipo == 1){         
       PORTCbits.RC1 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(tipo);
       temp3 = spiRead();
       
       __delay_ms(1);
       PORTCbits.RC1 = 1;       //Slave Deselect 
    }
       
    /**************escritura LCD**********************/
       
        escrituraLCD();
    }
    return;
}



//**********************************************************************************************
//Codigo de configuracion
//**********************************************************************************************
void setup(){
    
    configOsc(8); //configuracion del oscilador
    configPort(); //configuracion de puertos   
    pullup();
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    Lcd_Init();
    configIntbits();
    return;
}

/*******************************************************************/
//Funciones locales
/*******************************************************************/
void escrituraLCD(void){
    
  
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("S1:");
    Lcd_Set_Cursor(2,1);
    sprintf(ResSlave1,"%d",temp1);
    Lcd_Write_String(ResSlave1);
    
    Lcd_Set_Cursor(1,6);
    Lcd_Write_String("S2:");
    Lcd_Set_Cursor(2,6);
    sprintf(ResSlave2,"%d",temp2);
    Lcd_Write_String(ResSlave2);
    
    Lcd_Set_Cursor(1,11);
    Lcd_Write_String("S3:");
    Lcd_Set_Cursor(2,11);
    sprintf(ResSlave2C,"%d",temp3);
    Lcd_Write_String(ResSlave2C);
        
        __delay_ms(500);
    return;
}