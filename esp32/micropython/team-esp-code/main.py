# Derived from: 
# * https://github.com/peterhinch/micropython-async/blob/master/v3/as_demos/auart.py
# * https://github.com/tve/mqboard/blob/master/mqtt_async/hello-world.py


from mqtt_async import MQTTClient, config
import uasyncio as asyncio
import time
from machine import UART
import logging
import struct
import oled
from oled import display_oled
logging.basicConfig(level=logging.DEBUG)

MAXTX = 4

# Change the following configs to suit your environment
config.server = 'egr3x4.ddns.net' # can also be a hostname
config.ssid     = 'photon'
config.wifi_pw  = 'particle'


uart = UART(2, 9600,tx=17,rx=16)
uart.init(9600, bits=8, parity=None, stop=1,flow=0) # init with given parameters

MQTT_TOPIC_TEMP = "EGR314/team202/temp"
MQTT_TOPIC_HUM = "EGR314/team202/humidity"
MQTT_TOPIC_MODE = "EGR314/team202/current_mode"
MQTT_TOPIC_TIMER = "EGR314/team202/current_timer"
MQTT_TOPIC_DEBUG = "EGR314/team202/debug"

MQTT_TOPIC_SET_MODE = "EGR314/team202/set_mode"
MQTT_TOPIC_SET_TIMER = "EGR314/team202/set_timer"

mode_dict = {
                "AUTO": b'a',
                "TIMER": b't',
                "OFF": b'o'
            }

mode_to_str = ["OFF", "TIMER", "AUTO"]

current_temp = None
current_humidity = None
current_mode = None
current_timer = None

def update_screen():
    global mode_to_str, current_temp, current_humidity, current_mode, current_timer

    if not None in [current_temp, current_humidity, current_mode, current_timer]:
        mode_str = mode_to_str[current_mode]
        temp_f = int(current_temp * 9/5 + 32)
        
        timer_secs = int(current_timer % 60)
        timer_min = int((int(current_timer) - timer_secs) / 60)
        timer_formatted = "{:02d}:{:02d}".format(timer_min, timer_secs)
        humidity_percent = int(current_humidity)
        display_oled(temp_f, humidity_percent, timer_formatted, mode_str)
    else:
        print("Missing data: {}".format([current_temp, current_humidity, current_mode, current_timer]))

async def process_cmd(code, data):
    global current_temp, current_humidity, current_mode, current_timer

    data = data.decode().replace('X', '')
    code = code.decode()
    topic = ""
    if code == 'm':
        print("Mode: ", data)
        topic = MQTT_TOPIC_MODE
        current_mode = int(data)
        data = mode_to_str[int(data)]
    elif code == 'c':
        print("Time: ", data)
        topic = MQTT_TOPIC_TIMER
        current_timer = float(data)
    elif code == 't':
        print("Temp: ", data)
        topic = MQTT_TOPIC_TEMP
        try:
            current_temp = float(data)
            data = "{:.02f}".format(current_temp * 9/5 + 32)
        except ValueError:
            print("Error parsing temp")
            return
    elif code == 'h':
        print("Hum: ", data)
        topic = MQTT_TOPIC_HUM
        current_humidity = float(data)
        update_screen()
    elif code == 'd':
        print("Debug: ", data)
        topic = MQTT_TOPIC_DEBUG
    else:
        print("Unknown command: ", code, data)
        return
    
    print("Publishing to: ", topic, data)
    await client.publish(topic, data, qos=1)
    asyncio.sleep(0.2)

# Processes data from UART
async def receiver():
    b = b''
    sreader = asyncio.StreamReader(uart)
    while True:
        res = await sreader.read(1)
        print("Got char: ", res)
        if res in [b'm', b'c', b't', b'h', b'd']:
            data = b''
            while True:
                c = await sreader.read(1)
                data = data + c
                if c == b'X':
                    break
            print("Got command: ", res, data)
            #data = await sreader.read_until(b'X')
            await process_cmd(res, data)

def send_serial_cmd(data):
    #data = struct.pack("<sIs", character, value, b'X')
    print("Sending: ", data)
    uart.write(data)

# MQTT message callback
def callback(topic, msg, retained, qos):
    topic = topic.decode()
    if topic == MQTT_TOPIC_SET_MODE:
        msg = msg.decode()
        if msg.upper() in mode_dict:
            mode_char = mode_dict[msg.upper()]
            print("Set mode: ", msg)
            send_serial_cmd(mode_char)
    elif topic == MQTT_TOPIC_SET_TIMER:
        try:
            timer_val = int(msg)
        except ValueError:
            print("Error decoding number: ", msg)
            return
        print("Set timer: ", timer_val)
        timer_val = max(0, min(timer_val, 25))
        #timer_char = bytes(ord(timer_val + 65), 'ascii')
        send_serial_cmd(bytearray([timer_val+65]))
    else:
        print("Unknown topic: ", topic, msg)        
    time.sleep(.01)
  
async def conn_callback(client): 
    print("Connected to MQTT !!!\n\n")
    await client.subscribe(MQTT_TOPIC_SET_MODE, 1)
    await client.subscribe(MQTT_TOPIC_SET_TIMER, 1)

# Main program loop
async def main(client):
    await client.connect()
    asyncio.create_task(receiver())
    while True:
        await asyncio.sleep(1)

if __name__ == "__main__":
    config.subs_cb = callback
    config.connect_coro = conn_callback

    client = MQTTClient(config)
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main(client))
