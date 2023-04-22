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

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PROJECT_FUNCTIONS_H
#define	PROJECT_FUNCTIONS_H

#include "xc.h"

#include "mcc_generated_files/system.h"
#include "eepromI2C.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/i2c1.h"
#include "mcc_generated_files/tmr3.h"
#include "mcc_generated_files/mcc.h"
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

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
#define TEMP_SENSOR_ADDRESS 0x4C
#define TEMP_SENSOR_REG 0x00
#define HUMIDITY_SENSOR_ADDRESS 0x27
#define MOTOR_CONTROLLER_ADDRESS 0x60
#define MOTOR_CONTROLLER_CONTROL_REG 0x00
#define MOTOR_CONTROLLER_FAULT_REG 0x01
#define MOTOR_CONTROLLER_CLEAR_FAULTS 0x80

#define CMD_MODE 'm'
#define CMD_TIME 'c'

#define CMD_TEMP 't'
#define CMD_HUM 'h'    

#define MODE_OFF 0
#define MODE_TIMER 1
#define MODE_AUTO 2
#define HUMIDITY_SETPOINT_HIGH 30
#define HUMIDITY_SETPOINT_LOW 20
#define FAN_FWD 1
#define FAN_SPEED_LOW 30
#define FAN_SPEED_HIGH 60


#define TIMER_INCREMENT 60*5
#define TIMER_MAX 60*25
    

#define SERVO_POS_OFF 0
#define SERVO_POS_ON 600
void send_esp_packet_int(char, int);
void send_esp_packet_float(char, float);

void servo_timer_isr(void);

uint8_t I2C1_Read1ByteRegister(uint16_t, uint8_t);
void I2C1_Write1ByteRegister(uint16_t, uint8_t, uint8_t);
void I2C1_ReadNBytes(uint16_t, char*, int);
void I2C1_WriteNBytes(uint16_t, char* , int);

float get_temperature_c(void);
float get_humidity(void);

void set_fan(int8_t direction, uint8_t speed);
void set_servo(uint16_t position);


void isr_update(void);
void isr_off(void);
void isr_change_timer(void);
void isr_change_to_auto(void);

void update_screen(void);

//void blink(void);

int get_current_mode();
void set_timer(int16_t count);
void parse_serial_rx();

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

