
//temperature sensor variables
#include "DHT.h"
#include "LPD8806.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif
int nLEDs = 64;
#define DHTPIN 2  // temperature sensor to pin 2
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
//LED Variables
typedef union{
  float val;
  byte bytes[4];
} FLOATUNION_t;
byte RedBrightness = 127;
byte GreenBrightness = 0;
byte BlueBrightness = 127;
byte targetHumidity;
FLOATUNION_t targetTemp;
byte targetPh;

//constants for ledstrip
int dataPin = 3;
int clockPin = 4;
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);


//define variables for the perilstaltic pumps
int pHupPump = 10;
int pHdownPump = 11;
int nutrientsPump = 12;
int dropPeriod = 200;

///////WaterCycle////////
int submersiblePump = 6;
int solenoidValve = 5;


//define four states for water start from empty state
// empty - 0
//filling - 1
//full- 2
//emptying- 3
int waterState = 0;
unsigned long emptyTimer = 0;
unsigned long fullTimer = 0;

//set variables for ultrasonic sensor
int trig = 8;
int echo = 13;

//calibration settings
long full_level_raw = 5;
long empty_level_raw = 16;
unsigned long valvePulseTime = 500;
byte valveHighV = 170;
byte valveLowV = 85;

//user set levels
unsigned long emptyTimerLength = 600000; //time the top is empty
unsigned long fullTimerLength =  600000; //time the top is full
long full_level = 9;
long empty_level = 5;


void valveOn(int pin) {
  analogWrite(pin, valveHighV);
  delay(valvePulseTime);
  analogWrite(pin, valveLowV);
}

void resetWaterCycle() {
  if (getWaterLevel() > empty_level) {
      valveOn(solenoidValve);
      while (getWaterLevel() > empty_level) {
      } 
      analogWrite(solenoidValve, 0);
  }
  waterState = 0;
} 
long getWaterLevel () {
  long t = 0, h = 0, hp = 0;
  // Transmitting pulse
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Waiting for pulse
  t = pulseIn(echo, HIGH); 
  // Calculating distance 
  h = t / 58;
  return h;
  }
////////////////////////////////////////////////////////////////
  
void addConcentrate(int pin, int drops) {
  digitalWrite(pin,HIGH);
  delay(drops*dropPeriod);
  digitalWrite(pin,LOW);
  }
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pHupPump,OUTPUT); 
  pinMode(pHdownPump,OUTPUT); 
  pinMode(nutrientsPump,OUTPUT); 
//  pinMode(submersiblePump,OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT); 
  dht.begin();

   // Start up the LED strip
  strip.begin();
  // Update the strip, to start they are all 'off'
  strip.show();

}

//remember to change this once the temperature is working
void getTemp1() {
  float test = 2.0 ;
  for (int i =  0; i < sizeof(targetTemp.bytes); i++ ) {
      //Serial.write(targetTemp.bytes [i]);
  }
  Serial.println(test);
  
}
void getTemp() {
  FLOATUNION_t currentTemp;

  float t = dht.readTemperature(true);
  //try reading 2 times and if it fails send error
  int i = 0;
  while (i < 3 && isnan(t)) {
    delay(2000);
    t = dht.readTemperature(true);
    }

  //try reading 2 times and if it fails send error
//  int i = 0;
//  while (i < 3 && isnan(t)) {
//    delay(2000);
//    t = dht.readTemperature(true);
//    }
//     currentTemp.val = t;
//     for (int i =  0; i < sizeof(currentTemp.bytes); i++ ) {
//      Serial.write(targetTemp.bytes [i]);
//      }
  Serial.println(t);
  }
  
void getHumidity() {
  float h = dht.readHumidity();
  //try reading 2 times and if it fails send error
  int i = 0;
   Serial.println(h);
  }
  
void setTargetTemp() {
  byte temp[1];
  int bytesRead = Serial.readBytes(temp,1);
    if (bytesRead != 1) {
    Serial.println('N');
    }
    else {
    Serial.println('A');
    targetTemp.val = (float)temp[0];
    }
}

void setTargetColor() {
  byte  colors[3]; //first byte is red second is green third is blue
  int bytesRead = Serial.readBytes(colors,3);
  if (bytesRead != 3) {
    Serial.println('N');
    }
  else {
    Serial.println('A');
    RedBrightness = colors[0];
    GreenBrightness = colors[1];
    BlueBrightness = colors[2];
    }

  return;
  }


  
void setColor(int red, int green, int blue) {
   for (int i=0; i < strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(red,green,blue));  //turn all pixels on
        }
     strip.show(); 
}

void getColors() {
    //Serial.print("Red:");
    Serial.write(RedBrightness);
    //Serial.print("Green:",DEC);
    Serial.write(GreenBrightness);
    //Serial.print("Blue:");
    Serial.write(BlueBrightness);
  }
 
void loop() {

//check for communication from python/Raspberry Pi
if (Serial.available() > 0) {   
 byte func_byte = Serial.read();
    switch(func_byte) {
      case ('j'):
        setTargetColor();
        break;
      case('g'):
        getColors();
        break;
      case('t'):
        getTemp();
        break;
      case('T'):
        setTargetTemp();
        break;
       case('W'):
        //setWaterCycle();
        break;
      case('h'):
        getHumidity();
        break;
      default :
        while (Serial.available() > 0) {
          Serial.read();
          }
        break;
  }
  }
}
  
  void waterCycle() {
   int nextState = waterState;
   unsigned long currentMillis = millis();
  //empty state
   if (waterState == 0) {
    if (currentMillis - emptyTimer > emptyTimerLength) {
       //start the pump
       analogWrite(submersiblePump,255);
       //go to filling state
       nextState = 1; 
       delay(1);      
      }
   }
  //filling state
  else if (waterState == 1) {
    long waterLevel = getWaterLevel();
    //check transition
     delay(2);
    if ( waterLevel>= full_level) {
      //turn off pump
      analogWrite(submersiblePump,0);
      //transition to next state
      fullTimer = millis();
      //go to full state
      nextState = 2;
      }
  }
  //full state
  else if (waterState ==2) {
    if (currentMillis - fullTimer > fullTimerLength) {
      //transition
      valveOn(solenoidValve);
      nextState =3;
    }
  }
  //emptying state
  else if (waterState == 3) {
     long waterLevel = getWaterLevel();
     delay(1);
    if (waterLevel <= empty_level) {
      analogWrite(solenoidValve,0);
      emptyTimer = millis();
      nextState = 0; 
     
     }    
  }
  waterState = nextState;  
  
  }




