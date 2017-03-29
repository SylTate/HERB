import sys
import serial
import time
import struct
class ArduinoCom:

        def __init__(self,port,sleeptime = 2):
                self.ser = serial.Serial();
                try:
                        self.ser.port = '/dev/tty.usbserial-A1016O16'
                        self.ser.timeout = 3
                        self.ser.open()
                        time.sleep(2)
                        print "initializing serial commnuication"
                        if self.ser.is_open :
                             print "done... connected to serial port:"
                             print (self.ser.name)
                except serial.SerialException as e:
                     print "could not open the serial port"
                  

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

        #sends target RGB values to the arduino returns whether or not the write was succesfull 
        def setWaterCycle(self,emptyTime,fullTime) :
                 sentData = self.ser.write([b'W',chr(emptyTime),chr(fullTime)])
                 if sentData != 3 :
                         return false
                 line = self.ser.readline()
                 return ('A' in line)

        def setTemp(self,target_temp) :
                return (self.sendData([b'T',chr(target_temp)]))

        #remember to check the limits
        def setHumidity(self,target_humidity):
                return (self.sendData([b'H',chr(target_humidity)]))

        def setPH(self,target_humidity):
                return (sendData([b'P',chr(target_humidity)]))

        ##takes in integer which is the light period in minutes
        def setLightPeriod(self,light_period):
                light_period_string = 'L'+str(light_period) + '\n';
                sentData = self.ser.write(light_period_string)
                if (sentData != len(light_period_string)) :
                        return false;
                
                ##self.ser.write(light_period_string + '\n')
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
                        raise ValueError(sentData)
                for i in range(0,3) :
                       output.append(struct.unpack('B',self.ser.read(size=1))[0])
                return output

        def getTemp(self) :
                if self.ser.is_open:
                    sentData = self.ser.write('t')
                    if sentData != 1 :
                             raise ValueError(sentData)
                    output = self.ser.readline() 
                    #return struct.unpack('f',output)[0];
                    return output
                return 0.0

        def getHumidity(self) :
                if self.ser.is_open:
                        sentData = self.ser.write('h')
                        if sentData != 1 :
                                return None
                        if sentData != 1 :
                                 raise ValueError(sentData)
                        output = self.ser.readline()
                        return output
                return 0.0

def tests():
        comHandler = ArduinoCom("3")

        print comHandler.getLEDColors()
        for i in range(0,256,50):
                if comHandler.setLEDColors(i,i+1,i+2):
                           # print (ser.write(b'g'))
                           # print ser.read(size=1)
                           # print ser.read(size=1)
                           # print ser.read(size=1)a
                           print comHandler.getLEDColors()
                           time.sleep(3)
                else :
                        print "sending failed"
        #test sending and getting temperature
       # if comHandler.setTemp(50) :
        #        print "temp"
         #       print comHandler.getTemp();
          #      print "Humidity :"
           #     print comHandler.getHumidity();
        #test sending  getting humidity
        #if comHandler.setHumidity(50) :
         #       print "Humidity :"
          #      print comHandler.getHumidity();
        #comHandler.setLightPeriod(1);

if __name__ == '__main__':
        tests() 




         
