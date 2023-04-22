/*
 * File:   project_functions.c
 * Author: kk6ax
 *
 * Created on March 27, 2023, 7:31 PM
 */

#include "project_functions.h"

#ifndef FCY
#define FCY 16000000
#endif

#include <libpic30.h>

uint8_t current_mode = MODE_OFF;
uint32_t current_timer = 0;



void send_esp_packet_float(char cmd, float val){
    printf("$%c%.2fX\r", cmd, val);
}

void send_esp_packet_int(char cmd, int val){
    printf("$%c%dX\r", cmd, val);
}



volatile uint16_t servo_position = 1000;
void servo_timer_isr(){
    _LATB10 = 1;
    __delay_us(510);
    _LATB10 = 0;
    //LED1_Toggle();
}
//define addresses



// Setting the address consists of the output voltage for the first D7 to D2 digits then (0 1 for Reverse), (1 0 Forward), (1 1 Break), (0 0 standby)

/****   I2C Helper Functions    ****/

uint8_t I2C1_Read1ByteRegister(uint16_t address, uint8_t reg){
    return I2CReadReg(address, reg);
}

void I2C1_Write1ByteRegister(uint16_t address, uint8_t reg, uint8_t val){
    I2CWriteReg(address, reg, val);
}
void I2C1_ReadNBytes(uint16_t address, char *buf, int num_bytes){
    I2CSequentialReadReg(address, buf, num_bytes);
}

void I2C1_WriteNBytes(uint16_t address, char *buf, int num_bytes){
    I2CSequentialWriteReg(address, buf, num_bytes);
}


/****   Sensor and actuator functions    ****/
/**
 * Get the current temperature from the sensor.
 * @return temperature in degrees C.
 */
float get_temperature_c(void){
    float temp = I2C1_Read1ByteRegister(TEMP_SENSOR_ADDRESS, TEMP_SENSOR_REG);
    //printf("Temp %.0fC\n\r", temp);
    return temp;
}

/**
 * Get the current humidity from the sensor.
 * @return float humidity as a relative percentage 0-100.
 */
float get_humidity(void){
    char buffer[5] = {0, 0, 0, 0};
    I2C1_WriteNBytes(HUMIDITY_SENSOR_ADDRESS ,&buffer, 1);
    __delay_ms(50);
    I2C1_ReadNBytes(HUMIDITY_SENSOR_ADDRESS, &buffer, 4);
    uint8_t status = buffer[0]>>6;
    uint8_t humHi = buffer[0] & 0b00111111;
    uint8_t humLo = buffer[1];
    float humidity = humHi<<8 | humLo;
    float humidity_float = (humidity/16382.0) * 100.0;
    //printf("H: %0.2f%%RH\n\r",humidity_float);
    return humidity_float;
}

/**
 * Set the motor controller to a given speed. 
 * @param direction -1 for reverse, 1 for forward
 * @param speed integer speed from 0-63
 */
void set_fan(int8_t direction, uint8_t speed){
    // Limit speed to 63
    if(speed > 63){
        speed = 63;
    }
    uint8_t fault_reg = I2C1_Read1ByteRegister(MOTOR_CONTROLLER_ADDRESS, MOTOR_CONTROLLER_FAULT_REG);
    I2C1_Write1ByteRegister(MOTOR_CONTROLLER_ADDRESS, MOTOR_CONTROLLER_FAULT_REG, MOTOR_CONTROLLER_CLEAR_FAULTS);
    //printf("$d fault reg %dX", fault_reg);
    // The two LSB bits determine direction. The top 6 bits control speed.
    uint8_t dir_bits;
    if(speed == 0){
        dir_bits = 0b00;
    }else if(direction == -1){
        dir_bits = 0b01;
    }else if(direction == 1){
        dir_bits = 0b10;
    }
    
    // Combine the direction and speed bits together.
    uint8_t speed_byte = speed << 2 | dir_bits;
    I2C1_Write1ByteRegister(MOTOR_CONTROLLER_ADDRESS, MOTOR_CONTROLLER_CONTROL_REG, speed_byte);
    
}

/**
 * Set the servo to a given position.
 * @param position integer position from 1000-2000.
 */
void set_servo(uint16_t position){
    SERVO_SetHigh();
    LED1_SetHigh();
    __delay_us(position);
    SERVO_SetLow();
    LED1_SetLow();
}




void isr_update(void){
    if(current_mode == MODE_TIMER && current_timer > 0){
        current_timer -= 1;
    }
    
    // Maybe this is too long for an ISR, especially with the external communication.
    update_screen();
}

void isr_off(void){
    current_mode = MODE_OFF;
    current_timer = 0;
}

void isr_change_timer(void){
    current_mode = MODE_TIMER;
    // Increment the timer to the next increment, and remove any remainder.
    // This snaps the timer to a multiple of the increment.
    current_timer = (current_timer + TIMER_INCREMENT);
    
    // Wrap the timer back to 0 if it's past the max.
    if(current_timer > TIMER_MAX){
        current_timer = 0;
    }
    
    set_fan(FAN_FWD, FAN_SPEED_HIGH);
    set_servo(SERVO_POS_ON);
}
void isr_change_to_auto(void){
    current_mode = MODE_AUTO;
}

/**
 * Updates the ESP which will update the screen and MQTT
 */
void update_screen(void){
    send_esp_packet_int(CMD_TIME, current_timer);
    send_esp_packet_int(CMD_MODE, current_mode);
    send_esp_packet_float(CMD_TEMP, get_temperature_c());
    send_esp_packet_float(CMD_HUM, get_humidity());
}

void process_auto(float humidity){
    // Bang Bang controller with deadband
    if(humidity > HUMIDITY_SETPOINT_HIGH){
        set_servo(SERVO_POS_ON);
        set_fan(FAN_FWD, FAN_SPEED_HIGH);
    }else if(humidity < HUMIDITY_SETPOINT_LOW){
        set_servo(SERVO_POS_OFF);
        set_fan(FAN_FWD, FAN_SPEED_LOW);
    }
    
}

void process_timer(){
    set_servo(SERVO_POS_ON);
    if(current_timer <= 1){
        set_servo(SERVO_POS_OFF);
        current_mode = MODE_OFF;
    }
    
    __delay_ms(50);
}

void process_off(){
    set_servo(SERVO_POS_OFF);
    set_fan(FAN_FWD, FAN_SPEED_LOW);
    __delay_ms(50);
}

int get_current_mode(){
   return current_mode; 
}

void set_timer(int16_t count){
    current_timer = count;
}

#define RX_WAITING_HEADER 0
#define RX_GET_INT 1
#define RX_DONE 5


#define RX_MODE 0
#define RX_TIMER 1
int receive_status = 0;
int packet_type = 0;
char packet_data[5];
void parse_serial_rx(){
    //UART1_Receive_CallBack();
    if(UART1_IsRxReady()){
        if(receive_status == RX_WAITING_HEADER){
            char input_char = UART1_Read();
            if(input_char == 'm'){
                // This is a mode command
                packet_type = RX_MODE;
                receive_status = RX_GET_INT;
            }else if (input_char == 't'){
                // This is a timer command
                packet_type = RX_TIMER;
                receive_status = RX_GET_INT;
            }else{
                receive_status = RX_WAITING_HEADER;
            }
        }
        
        if(receive_status != RX_WAITING_HEADER){
            // We've received a header, so process the body or ending
            if(receive_status != RX_DONE){
                LED3_Toggle();
                packet_data[receive_status-1] = UART1_Read();
                // Increment to receive the next byte
                receive_status += 1;
                printf("d Packet %dX\r", receive_status);
            }else{
                // This is the ending byte
                if(UART1_Read() == 'X'){
                    // This is the correct ending byte
                    // Convert bytes to unsigned int
                    unsigned int res = 0;
                    res = res + (packet_data[0]      );
                    res = res + (packet_data[1] << 8 );
                    res = res + (packet_data[2] << 16);
                    res = res + (packet_data[3] << 24);
                    
                    
                    if(packet_type == RX_TIMER){
                        printf("$d New timer is %dX", res);
                    }else if(packet_type == RX_MODE){
                        printf("$d New mode is %dX", res);
                    }
                    
                    
                }else{
                    printf("$d Error parsing packet...X");
                }
                receive_status = RX_WAITING_HEADER;
            }
        }
    }
}