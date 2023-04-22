/*
 * File:   eepromI2C.c
 * Author: Icaro J.
 *
 * Created on 20 de Janeiro de 2017, 15:30
 */
 
 /* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */
  
// This is a guard condition so that contents of this file are not included
// more than once.  

#ifndef XC_HEADER_TEMPLATE_H
#define XC_HEADER_TEMPLATE_H
 
#include <xc.h> // include processor files - each processor file is guarded.  
 
// TODO Insert appropriate #include <>
 
// TODO Insert C++ class definitions if appropriate
 
// TODO Insert declarations
 
// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
   
    <p><b>Summary:</b></p>
 
    <p><b>Description:</b></p>
 
    <p><b>Precondition:</b></p>
 
    <p><b>Parameters:</b></p>
 
    <p><b>Returns:</b></p>
 
    <p><b>Example:</b></p>
    <code>
  
    </code>
 
    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation
 
#ifdef  __cplusplus
extern "C" {
#endif /* __cplusplus */
 
//Global Vars
#define EEPROMaddr 80
#define EEPROMbaud 23 // @FSCL = 400kHz; (FCY/FSCL - FCY/1e7) - 1
//#define EEPROMbaud 98 // @FSCL = 100kHz;  
//
extern void I2CInit(void);
extern void I2CStart(void);
extern void I2CRestart(void);
extern void I2CStop(void);
extern void I2CWaitACK(void);
extern void I2CIdle(void);
extern void I2CAck(void);
extern void I2CNack(void);
extern void I2CWrite(unsigned char c);
extern void I2CRead(void);
extern void I2CWriteReg(char addr, char byteHigh, char value);
extern char I2CReadReg(char addr, char byteHigh);//, char byteLow);
extern void I2CSequentialWriteReg(char addr, char* value, int length);
extern void I2CSequentialReadReg(char addr, char* buff, int length);
 
#ifdef  __cplusplus
}
#endif /* __cplusplus */
 
#endif  /* XC_HEADER_TEMPLATE_H */