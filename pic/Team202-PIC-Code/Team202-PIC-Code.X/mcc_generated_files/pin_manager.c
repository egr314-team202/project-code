/**
  PIN MANAGER Generated Driver File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.c

  @Summary:
    This is the generated manager file for the PIC24 / dsPIC33 / PIC32MM MCUs device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description:
    This source file provides implementations for PIN MANAGER.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.1
        Device            :  PIC24FJ64GA702
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v5.50
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/


/**
    Section: Includes
*/

#include <xc.h>
#include <stdio.h>
#include "pin_manager.h"

/**
 Section: File specific functions
*/
void (*BUTTON2_InterruptHandler)(void) = NULL;
void (*BUTTON3_InterruptHandler)(void) = NULL;
void (*BUTTON1_InterruptHandler)(void) = NULL;

/**
 Section: Driver Interface Function Definitions
*/
void PIN_MANAGER_Initialize (void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000;
    LATB = 0x0000;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x0001;
    TRISB = 0x7BEF;

    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    IOCPDA = 0x0000;
    IOCPDB = 0x0000;
    IOCPUA = 0x0000;
    IOCPUB = 0x0000;

    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000;
    ODCB = 0x0000;

    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSA = 0x0000;
    ANSB = 0x0000;
    
    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    RPOR13bits.RP27R = 0x0005;    //RA1->UART2:U2TX
    RPINR18bits.U1RXR = 0x000E;    //RB14->UART1:U1RX
    RPINR19bits.U2RXR = 0x001A;    //RA0->UART2:U2RX
    RPOR7bits.RP15R = 0x0003;    //RB15->UART1:U1TX

    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
    
    /****************************************************************************
     * Interrupt On Change: negative
     ***************************************************************************/
    IOCNBbits.IOCNB11 = 1;    //Pin : RB11
    IOCNBbits.IOCNB12 = 1;    //Pin : RB12
    IOCNBbits.IOCNB13 = 1;    //Pin : RB13
    /****************************************************************************
     * Interrupt On Change: flag
     ***************************************************************************/
    IOCFBbits.IOCFB11 = 0;    //Pin : RB11
    IOCFBbits.IOCFB12 = 0;    //Pin : RB12
    IOCFBbits.IOCFB13 = 0;    //Pin : RB13
    /****************************************************************************
     * Interrupt On Change: config
     ***************************************************************************/
    PADCONbits.IOCON = 1;    //Config for PORTB
    
    /* Initialize IOC Interrupt Handler*/
    BUTTON2_SetInterruptHandler(&BUTTON2_CallBack);
    BUTTON3_SetInterruptHandler(&BUTTON3_CallBack);
    BUTTON1_SetInterruptHandler(&BUTTON1_CallBack);
    
    /****************************************************************************
     * Interrupt On Change: Interrupt Enable
     ***************************************************************************/
    IFS1bits.IOCIF = 0; //Clear IOCI interrupt flag
    IEC1bits.IOCIE = 1; //Enable IOCI interrupt
}

void __attribute__ ((weak)) BUTTON2_CallBack(void)
{

}

void __attribute__ ((weak)) BUTTON3_CallBack(void)
{

}

void __attribute__ ((weak)) BUTTON1_CallBack(void)
{

}

void BUTTON2_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC1bits.IOCIE = 0; //Disable IOCI interrupt
    BUTTON2_InterruptHandler = InterruptHandler; 
    IEC1bits.IOCIE = 1; //Enable IOCI interrupt
}

void BUTTON2_SetIOCInterruptHandler(void *handler)
{ 
    BUTTON2_SetInterruptHandler(handler);
}

void BUTTON3_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC1bits.IOCIE = 0; //Disable IOCI interrupt
    BUTTON3_InterruptHandler = InterruptHandler; 
    IEC1bits.IOCIE = 1; //Enable IOCI interrupt
}

void BUTTON3_SetIOCInterruptHandler(void *handler)
{ 
    BUTTON3_SetInterruptHandler(handler);
}

void BUTTON1_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC1bits.IOCIE = 0; //Disable IOCI interrupt
    BUTTON1_InterruptHandler = InterruptHandler; 
    IEC1bits.IOCIE = 1; //Enable IOCI interrupt
}

void BUTTON1_SetIOCInterruptHandler(void *handler)
{ 
    BUTTON1_SetInterruptHandler(handler);
}

/* Interrupt service routine for the IOCI interrupt. */
void __attribute__ (( interrupt, no_auto_psv )) _IOCInterrupt ( void )
{
    if(IFS1bits.IOCIF == 1)
    {
        if(IOCFBbits.IOCFB12 == 1)
        {
            if(BUTTON2_InterruptHandler) 
            { 
                BUTTON2_InterruptHandler(); 
            }
            
            IOCFBbits.IOCFB12 = 0;  //Clear flag for Pin - RB12

        }
        
        if(IOCFBbits.IOCFB13 == 1)
        {
            if(BUTTON3_InterruptHandler) 
            { 
                BUTTON3_InterruptHandler(); 
            }
            
            IOCFBbits.IOCFB13 = 0;  //Clear flag for Pin - RB13

        }
        
        if(IOCFBbits.IOCFB11 == 1)
        {
            if(BUTTON1_InterruptHandler) 
            { 
                BUTTON1_InterruptHandler(); 
            }
            
            IOCFBbits.IOCFB11 = 0;  //Clear flag for Pin - RB11

        }
        
        
        // Clear the flag
        IFS1bits.IOCIF = 0;
    }
}

