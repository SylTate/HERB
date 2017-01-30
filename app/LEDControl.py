import sys
import serial
import time
import struct
class ArduinoLEDCom:

        def __init__(self,port,sleeptime = 2):
                self.ser = serial.Serial('/dev/tty.usbserial-A1016O16')
                print "initializing serial commnuication"
                time.sleep(2)
                if self.ser.is_open :
                        print "done... connected to serial port:"
                        print (self.ser.name)

        def packIntegerAsULong(value):
            """Packs a python 4 byte unsigned integer to an arduino unsigned long"""
            return struct.pack('I', value)    #should check bounds


        #sends target RGB values to the arduino returns whether or not the write wassuccesfull 
        def setLEDColors(self,red, green, blue) :
                 sentData = self.ser.write([b'j',chr(red),chr(green),chr(blue)])
                 if sentData != 4 :
                         return false
                 line = self.ser.readline()
                 return ('A' in line)

        #TODO : figure out the timeouts needed for the read operations to avoid
        #infinite loop. Also maybe throw exceptions instead of null
        #This function returns a list with the the first index as the value for Red
        # the second value in the array is Blue and third is Green
        #returns a null array if an error occured
        def getLEDColors(self) :
                output = []
                sentData = self.ser.write('g')
                if sentData != 1 :
                        return None
                for i in range(0,3) :
                       # line = ser.readline()
                       # output.append(list(line.split(':',1)[1])[0:3])
                       output.append(struct.unpack('B',self.ser.read(size=1))[0])
                return output

def tests():
        comHandler = ArduinoLEDCom("3")

        print comHandler.getLEDColors()
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
                           print comHandler.getLEDColors()
                           time.sleep(5)
                else :
                        print "sending failed"
               # print bytes(line)
                #line = ser.readline()
               # print line
                #line = ser.readline()
                #print line

if __name__ == '__main__':
        tests()




         
