import sys
import serial
import time
import struct


class ArduinoLEDCom:

        def __init__(self,port,sleeptime = 2):

                self.ser = serial.Serial('/dev/tty.usbserial-A1016O16',timeout = 10)
                print( "initializing serial commnuication")
                time.sleep(2)
                if self.ser.is_open :
                        print ("done... connected to serial port:")
                        print (self.ser.name)

        def packIntegerAsULong(value):
            """Packs a python 4 byte unsigned integer to an arduino unsigned long"""
            return struct.pack('I', value)    #should check bounds

        def sendData(self,data) :
                sentData = self.ser.write(data)
                if sentData != len(data):
                        return false
                line = self.ser.readline()
                return ('A' in line)


        #sends target RGB values to the arduino returns whether or not the write was succesfull
        def setLEDColors(self,red, green, blue) :
                 sentData = self.ser.write([b'j',chr(red),chr(green),chr(blue)])
                 if sentData != 4 :
                         return false
                 line = self.ser.readline()
                 return ('A' in line)

        def setTemp(self,target_temp) :
                return (self.sendData([b'T',chr(target_temp)]))

        #remember to check the limits
        def setHumidity(self,target_humidity):
                return (sendData([b'T',chr(target_humidity)]))

        def setPH(self,target_humidity):
                return (sendData([b'P',chr(target_humidity)]))


        #TODO : figure out the timeouts needed for the read operations to avoid
        #infinite loop. Also maybe throw exceptions instead of null
        #This function returns a list with the the first index as the value for Red
        # the second value in the array is Blue and third is Green
        #returns a null array if an error occured
        def getLEDColors(self) :
                output = []
                sentData = self.ser.write('g')
                if sentData != 1 :
                        raise ValueError(sentData)
                for i in range(0,3) :
                       output.append(struct.unpack('B',self.ser.read(size=1))[0])
                return output

        def getTemp(self) :
                sentData = self.ser.write('t')
                if sentData != 1 :
                         raise ValueError(sentData)
                output = self.ser.read(size = 4) ;
                print(output)
                print(len(output));
                return struct.unpack('f',output)[0];

        def getHumidity(self) :
                sentData = self.ser.write('t')
                if sentData != 1 :
                        return None
                return struct.unpack('f',self.ser.read(size = 4))[0];

def tests():
        comHandler = ArduinoLEDCom("3")
        print( comHandler.getLEDColors())
        for i in range(0,255,30):

               # ser = serial.Serial('/dev/tty.usbserial-A1016O16')
               # time.sleep(2)
               # print ser.write([b'j',chr(i),chr(i+1),chr(i+2)])

               # time.sleep(.01)
                #print(ser.write(chr(i+1)))
               # while ser.inWaiting() < 1:
                #        print "waiting for response \n"

               # line = ser.readline()
               # print list(line)
                if comHandler.setLEDColors(i,i+1,i+2):
                           # print (ser.write(b'g'))
                           # print ser.read(size=1)
                           # print ser.read(size=1)
                           # print ser.read(size=1)a
                           print (comHandler.getLEDColors())
                           time.sleep(5)
                else :

                        print ("sending failed")
               # print bytes(line)
                #line = ser.readline()
               # print line
                #line = ser.readline()
                #print line
                 #test sending and getting temperature
        if comHandler.setTemp(50) :
                print( comHandler.getTemp())

if __name__ == '__main__':
        tests()





