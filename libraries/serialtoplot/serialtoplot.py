"""
 serialtoplot Module

 UCLA Rocket Project, 2014-2015
 Written by Ky-Cuong L. Huynh
 Last updated: 5 March 2015

"""

"""
See https://github.com/UCLARocketProject/2014-2015/wiki/Specification:-serialtoplot
for the latest specification of this module's behavior. 

Compatibility: Python 2.7.x or greater.

"""

import serial   # PySerial for USB serial communication

from __future__ import print_function # Compatibility with Python 3.x

class SerialRadio(object):

    # Initialize a SerialRadio object to read from
    # a connected radio.
    def __init__(self, serialPort, timeout):
        try:
            # Initialize a new serial port,
            # but don't try to open it yet
            self.ser = serial.Serial()
            self.ser.port = serialPort
            self.ser.baudrate = 19200
            self.ser.timeout = timeout
        except ValueError:
            print("Invalid options for new SerialRadio.")
        except SerialException:
            print("SerialException from PySerial. Is the radio connected?")
        except:
            print("Unknown error in SerialRadio initialization.")
        else:
            self.ready = True

    # Begin reading from the serial port
    def beginSerialRead(self):
        try:
            self.ser.open()
        except SerialException:
            print("SerialException from PySerial. Is the radio connected?")
        except:
            print("Unknown error in SerialRadio initialization.")

    # End reading from the serial port
    def endSerialRead(self):
        try:
            self.ser.close()
            self.ready = False
        except: 
            print("Unknown error while trying to close port.")

    # Returns True if ready and False otherwise
    def isReady(self):
        return self.ready

    # TODO: Stub
    def getAccel(self):
        return None

    def getOrientation(self): 
        return None


# TODO: No unit tests yet
def unitTests():
    pass


# TODO: Add more commandline testing options as needed
if __name__ == "__main__":
    import sys # For commandline options
    unitTests()

