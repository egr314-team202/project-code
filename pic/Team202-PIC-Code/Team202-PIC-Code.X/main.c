/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
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
  Section: Included Files
*/
#define FCY 16000000
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/i2c1.h"
#include "mcc_generated_files/tmr3.h"
#include "mcc_generated_files/mcc.h"
#include <stdio.h>
#include <libpic30.h>
#include "eepromI2C.h"
#include "project_functions.h"

#define SERVO_SWITCH_ON 2000
#define SERVO_SWITCH_OFF 1000

#define HUM_SETPOINT 30

// This is the bang bang controller for humidity control. 
/*
void controller(){
    if(get_humidity() > HUM_SETPOINT){
        //set_fan(FAN_SPEED_HIGH);
        set_servo(SERVO_SWITCH_ON);
    }else{
        //set_fan(FAN_SPEED_LOW);
        set_servo(SERVO_SWITCH_OFF);
    }
}*/

void test_mode(){
    if (BUTTON1_GetValue()) {
        LED2_SetHigh();

    } else {
        LED2_SetLow();
    }
    if (BUTTON2_GetValue()) {
        LED3_SetHigh();

    } else {
        LED3_SetLow();
    }
    if (BUTTON3_GetValue()) {
        LED4_SetHigh();

    } else {
        LED4_SetLow();
    }
    //LED1_Toggle();
    //printf("Start\r\n");
    //I2C1_Read1ByteRegister(ADDRESS, 0x01);
    get_temperature_c();
    get_humidity();

    set_fan(1, 63);
    //printf("Temp: %d", get_temperature_c());
    //printf("Humidity: %d")
    update_screen();
    //printf("End update screen\r");
    __delay_ms(2000);
    set_fan(1, 0);
    __delay_ms(1000);
    //printf("End delay\r");
}

void test_esp32(){
    printf("Sending byte");
    while(!UART1_IsTxReady());
    
    UART1_Write('a');
    
    while(!UART1_IsTxDone());
    
    printf("Done sending");
    
    if(UART1_IsRxReady()){
        uint8_t input_char = UART1_Read();
        printf("Got: %d", input_char);
    }
}

/*
Main application
*/
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    // 1 Hz timer
    TMR3_Start();
    TMR3_SetInterruptHandler(isr_update);
    TMR2_Start();
    
    // UI Buttons
    BUTTON1_SetInterruptHandler(isr_off);
    BUTTON2_SetInterruptHandler(isr_change_timer);
    BUTTON3_SetInterruptHandler(isr_change_to_auto);
    
    // Servo
    //TMR2_SetInterruptHandler(servo_timer_isr);
    //__delay_ms(1000);
    printf("Boot\r");
    //__delay_ms(100);
    //test_mode();
    while (1) {
        //printf("Start loop \r");
        int temp = get_temperature_c();
        float humidity = get_humidity();
        int current_mode = get_current_mode();
        if(current_mode == MODE_TIMER){
            process_timer();
        }else if(current_mode == MODE_AUTO){
            process_auto(humidity);
        }else{
            process_off();
        }
        
        //parse_serial_rx();
        if(UART1_IsRxReady()){
            char data = UART1_Read();
            if(data == 'o'){
                isr_off();
                
            }else if(data == 't'){
                isr_change_timer();
            }else if(data == 'a'){
                isr_change_to_auto();
            }else if(data > 64 && data < 91){
                LED4_Toggle();
                set_timer((data-65) * 60);
            }
        }
        __delay_ms(100); // Slow down main loop
    }

    return 1;
}
/**
 End of File
*/

