import time
import serial
import atexit
ser = serial.Serial('/dev/ttyUSB1', 115200)

time.sleep(2)  # gives the Serial Port time to open 

ser.flushInput() # flushes the serial buffer
ser.flushOutput() 
import bpy

import struct # module that packs data into byte sized chunks to be sent over serial


import bpy


class Com:
    def __init__(self, count):
        self.count = count
        self.timeOfLastCall = time.time()
        self.currntTime = time.time()
    
    def growCount(self):
        self.currentTime = time.time()
        self.callsPerSecond = 1/ (time.time() - self.timeOfLastCall)
        self.timeOfLastCall = time.time()
        
        return self.callsPerSecond
    
com1 = Com(1)


def my_handler(scene):
    print("Frame Change", scene.frame_current)
    print(int(bpy.data.objects['Cube'].location.y))    
    print(com1.growCount())
    #print(time.time())
    #mesage = str(int(bpy.data.objects['Cube'].location.y)).encode()
    intMesage = int(bpy.data.objects['Cube'].location.y * 100.0)
    mesage = struct.pack('H', (intMesage))
    ser.write(mesage)
    #print(ser.read())
    print(mesage)
    print(intMesage)
    
    
bpy.app.handlers.frame_change_pre.append(my_handler)
bpy.app.handlers.scene_update_post.append(my_handler)

def doAtExit():
    ser.close()
    print("exited with Port CLosed")
atexit.register(doAtExit)