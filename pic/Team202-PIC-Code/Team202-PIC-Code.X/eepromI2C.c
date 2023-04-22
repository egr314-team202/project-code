/*
 * File:   eepromI2C.c
 * Author: Icaro J.
 *
 * Created on 20 de Janeiro de 2017, 15:30
 */
 
 
#include "xc.h"
#include "eepromI2C.h"
#define FCY 16000000

#include <libpic30.h>

void I2CInit(void)
{
    I2C1BRG = EEPROMbaud;           // @400kHz; (FCY/FSCL - FCY/1e7) - 1
    I2C1CONLbits.I2CEN = 0;  // Disable I2C
    I2C1CONLbits.DISSLW = 1; // Disable slew rate control
    I2C1CONLbits.A10M = 0;   // 7-bit slave addr
    I2C1CONLbits.SCLREL = 1; // SCL release control
    I2C1CONLbits.I2CEN = 1;  // Enable I2C
    IEC1bits.MI2C1IE = 0;   // Master I2C interrupt
    IFS1bits.MI2C1IF = 0;   // MI2C Flag
}
 
void I2CAck(void)
{
    I2C1CONLbits.ACKDT = 0;      // Send ACK
    I2C1CONLbits.ACKEN = 1;      // Initiate Acknowledge and transmit ACKDT
    while(I2C1CONLbits.ACKEN);
}
 
void I2CNack(void)
{
    I2C1CONLbits.ACKDT = 1;      // Send NACK
    I2C1CONLbits.ACKEN = 1;      // Initiate Acknowledge and transmit ACKDT
    while(I2C1CONLbits.ACKEN);  
}
 
void I2CStop(void)
{
    I2C1CONLbits.RCEN = 0;       // receive mode not in progress
    I2C1CONLbits.PEN = 1;        // Stop condition
    while(I2C1CONLbits.PEN);
}
 
void I2CStart(void)
{
    I2C1CONLbits.ACKDT = 0;      // Reset any ACK
    I2C1CONLbits.SEN = 1;        // Start
    while(I2C1CONLbits.SEN);
}
 
void I2CRestart(void)
{
    I2C1CONLbits.RSEN = 1;       // Repeated Start Condition
    while(I2C1CONLbits.RSEN);
    I2C1CONLbits.ACKDT = 0;      // Send ACK
    I2C1STATbits.TBF = 0;       // I2C1TRN is empty
}
 
void I2CWaitACK(void)
{
    while(I2C1STATbits.ACKSTAT);
}
 
void I2CIdle(void)
{
    while(I2C1STATbits.TRSTAT);
}
 
void I2CWrite(unsigned char c)
{
    I2C1TRN = c;
    while(I2C1STATbits.TBF);
}
 
void I2CRead(void)
{
    I2C1CONLbits.RCEN = 1;
    Nop();
    while(!I2C1STATbits.RBF);
}
 
void I2CWriteReg(char addr, char byteHigh, char value)
{
    // Start Condition
    I2CStart();
    // EEPROM Addr
    I2CWrite((addr<<1)&0xFE);
    I2CIdle();
    // Addr High Byte
    I2CWrite(byteHigh);
    I2CIdle();
    // Addr Low Byte
    //I2CWrite(byteLow);
    //I2CIdle();
    // Value
    I2CWrite(value);
    I2CIdle();
    // Stop
    I2CStop();
}
 
 
char I2CReadReg(char addr, char byteHigh)//, char byteLow)
{
    char temp;
    // Start Condition
    I2CStart();
    // EEPROM Addr
    I2CWrite((addr<<1)&0xFE);
    I2CIdle();
    // Addr High Byte
    I2CWrite(byteHigh);
    I2CIdle();
    // Addr Low Byte
    //I2CWrite(byteLow);
    //I2CIdle();
    // Restart
    I2CRestart();
    I2CWrite((addr<<1)|0x01);
    I2CIdle();  
    I2CRead();
    I2CNack();
    I2CStop();
    temp = I2C1RCV;
    return temp;
}
 
void I2CSequentialWriteReg(char addr, char* value, int length)
{
    int j;
    // Start Condition
    I2CStart();
    // EEPROM Addr
    I2CWrite((addr<<1)&0xFE);
    I2CIdle();
    // Addr High Byte
    //I2CWrite(byteHigh);
    //I2CIdle();
    // Addr Low Byte
    //I2CWrite(byteLow);
    //I2CIdle();
    // Value
    for(j = 0; j < length; j++)
    {
        I2CWrite(value[j]);
        I2CIdle();
    }
    // Stop
    I2CStop();    
}
 

// Doesn't send a register address, just attempts to read n bytes
void I2CSequentialReadReg(char addr, char* buff, int length)
{
    int j;
    // Start Condition
    I2CStart();
    // EEPROM Addr
    //I2CWrite((addr<<1)&0xFE);
    //I2CIdle();
    // Addr High Byte
    //I2CWrite(byteHigh);
    //I2CIdle();
    // Addr Low Byte
    //I2CWrite(byteLow);
    //I2CIdle();
    // Restart
    //I2CRestart();
    I2CWrite((addr<<1)|0x01);
    I2CIdle();  
    for(j = 0; j < length; j++)
    {
        I2CRead();
        buff[j] = I2C1RCV;
        // Don't ACK on the last byte, send a NACK once we break
        if(j != length -1){
            I2CAck();
        }
    }
    buff[j] = '\0';
    I2CNack();
    I2CStop();  
}