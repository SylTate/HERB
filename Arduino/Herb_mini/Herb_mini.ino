
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
typedef union{
  float val;
  byte bytes[4];
} FLOATUNION_t;
/***************************PINS*****************************/
///constants for ledstrip
int dataPin = 3;
int clockPin = 4;
int solenoidValve = 5;
int submersiblePump = 6;

//set variables for ultrasonic sensor
int trig = 8;
int echo = 9;
//define variables for the perilstaltic pumps
int pHupPump = 10;
int pHdownPump = 11;
int nutrientsPump = 12;


LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);
/********************LED Variables*********************************/
byte RedBrightness = 127;
byte GreenBrightness = 0;
byte BlueBrightness = 127;
unsigned long lightPeriod = 120000;
unsigned long lightTimer = 0;

/******************Target environment variabels********************/
byte targetHumidity;
FLOATUNION_t targetTemp;
byte targetPh;
int dropPeriod = 200; //calibration for periltaltic pumps

/********************Water Cycle*********************************/
//define four states for water start from empty state
int waterState = 0; // empty - 0, filling - 1 ,full- 2, emptying- 3
unsigned long emptyTimer = 0;
unsigned long fullTimer = 0;

//calibration settings
long full_level_raw = 5;
long empty_level_raw = 16;
unsigned long valvePulseTime = 500;
byte valveHighV = 170;
byte valveLowV = 85;

//user set levels
unsigned long emptyTimerLength_minutes = 45;
unsigned long emptyTimerLength = emptyTimerLength_minutes*60*1000; //time the top is empty
unsigned long fullTimerLength =  10000; //time the top is full
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
  delay(10);
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
  h = h - full_level_raw;
  h = empty_level_raw - full_level_raw - h;
  return h;
}

  
void addConcentrate(int pin, int drops) {
  digitalWrite(pin,HIGH);
  delay(drops*dropPeriod);
  digitalWrite(pin,LOW);
}
/****************Sensors***************/
void getTemp() {
  FLOATUNION_t currentTemp;
  float t = dht.readTemperature(true);
  //try reading 2 times and if it fails send error
  int i = 0;
  while (i < 3 && isnan(t)) {
    delay(2000);
    t = dht.readTemperature(true);
    }
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

/**********************Lights***************************/

void initializeLightTimer() {
  lightTimer = millis();
  setColor((int)RedBrightness,(int)GreenBrightness,(int)BlueBrightness);
 }
void setLightPeriod() {
     String periodRead = Serial.readStringUntil('\n'); // minutes
     int periodMinutes = periodRead.toInt();
     if (periodMinutes == 0) {
        Serial.println('N');
      }
      else {
        Serial.println('A');
        lightPeriod = periodMinutes * 60000;
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
    Serial.write(RedBrightness);
    Serial.write(GreenBrightness);
    Serial.write(BlueBrightness);
  }
   
void setup() {
    Serial.begin(9600);
    pinMode(pHupPump,OUTPUT); 
    pinMode(pHdownPump,OUTPUT); 
    pinMode(nutrientsPump,OUTPUT); 
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT); 
    dht.begin();  //start the DHT sensor
    strip.begin(); //start the led Strip
    strip.show(); // Update the strip, to start they are all 'off'
    setColor((int)RedBrightness,(int)GreenBrightness,(int)BlueBrightness);
    resetWaterCycle();
    
}


void loop() {
communication();
waterCycle();

}

/*Handle serial communication with Raspberry Pi*/
void communication() {
 if (millis() - lightTimer > lightPeriod) {
     setColor(0,0,0);
  }
  if (Serial.available() > 0) {   
     byte func_byte = Serial.read();
        switch(func_byte) {
          case ('j'):
            setTargetColor();
            setColor((int)RedBrightness,(int)GreenBrightness,(int)BlueBrightness);
            break;
          case('g'):
            getColors();
            break;
          case('t'):
           // getTemp();
            break;
          case('T'):
    //        setTargetTemp();
            break;
           case('W'):
            //setWaterCycle();
            break;
          case('h'):
            //getHumidity();
            break;
          case('F'):
            fancyDemo();
            break;
          case('L'):
            setLightPeriod();
            initializeLightTimer();
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

 void fancyDemo() {

  // Send a simple pixel chase in...
  colorChase(strip.Color(127, 127, 127), 50); // White
  colorChase(strip.Color(127,   0,   0), 50); // Red
  colorChase(strip.Color(127, 127,   0), 50); // Yellow
  colorChase(strip.Color(  0, 127,   0), 50); // Green
  colorChase(strip.Color(  0, 127, 127), 50); // Cyan
  colorChase(strip.Color(  0,   0, 127), 50); // Blue
  colorChase(strip.Color(127,   0, 127), 50); // Violet

  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127,   0,   0), 50); // Red
  theaterChase(strip.Color(127, 127,   0), 50); // Yellow
  theaterChase(strip.Color(  0, 127,   0), 50); // Green
  theaterChase(strip.Color(  0, 127, 127), 50); // Cyan
  theaterChase(strip.Color(  0,   0, 127), 50); // Blue
  theaterChase(strip.Color(127,   0, 127), 50); // Violet

  // Fill the entire strip with...
  colorWipe(strip.Color(127,   0,   0), 50);  // Red
  colorWipe(strip.Color(  0, 127,   0), 50);  // Green
  colorWipe(strip.Color(  0,   0, 127), 50);  // Blue

  rainbow(10);
  rainbowCycle(0);  // make it go through the cycle fairly fast
  theaterChaseRainbow(50);
}

void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 384; j++) {     // 3 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 384));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  
  for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 384-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 384 / strip.numPixels()) + j) % 384) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Fill the dots progressively along the strip.
void colorWipe(uint32_t c, uint8_t wait) {
  int i;

  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// Chase one dot down the full strip.
void colorChase(uint32_t c, uint8_t wait) {
  int i;

  // Start by turning all pixels off:
  for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);

  // Then display one pixel at a time:
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
    delay(wait);
  }

  strip.show(); // Refresh to turn off last pixel
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 384; j++) {     // cycle all 384 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 384));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}
/* Helper functions */

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r

uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      b = 127 - WheelPos % 128;  //blue down 
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break; 
  }
  return(strip.Color(r,g,b));
}

 

