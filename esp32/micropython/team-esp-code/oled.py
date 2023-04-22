from machine import Pin, SoftI2C
import ssd1306
from time import sleep

# ESP32 Pin assignment
i2c = SoftI2C(scl=Pin(22), sda=Pin(21))

oled_width = 128
oled_height = 64
oled = ssd1306.SSD1306_I2C(oled_width, oled_height, i2c)

def get_temp(temp):
   
    temp_str = str(temp)
    return temp_str

def get_humidity(hum):
   
    hum_str = str(hum)
    return hum_str

def get_timer(timer):
   
    timer_str = str(timer)
    return timer_str

def get_mode(mode):
    return mode
   

def display_oled(temp, humidity, timer, mode):
    oled.fill(0)
    print("Update screen temp {}, hum {}, timer {}, mode {}".format(temp, humidity, timer, mode))
    oled.text('Temp: {} F'.format(temp), 0, 0)
    oled.text('Humidity: {}%'.format(humidity), 0, 10)
    oled.text('Timer: {}'.format(timer), 0, 20)
    oled.text('Mode: {}'.format(mode), 0, 30)
    oled.show()
