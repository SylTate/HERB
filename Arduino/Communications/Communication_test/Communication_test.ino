#include "LPD8806.h"
int nLEDs = 64;
int dataPin = 3;
int clockPin = 4;
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

//Target Variables
byte RedBrightness = 127;
byte GreenBrightness = 0;
byte BlueBrightness = 127;
unsigned long lightPeriod = 120000;
unsigned long lightTimer = 0;
void setTargetColor() {
  byte  colors[3]; //first byte is red second is green third is blue
  int bytesRead = Serial.readBytes(colors,3);
  if (bytesRead != 3) {
    //Serial.println('N');
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
    //Serial.println("updating");
     for (int i=0; i < strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(red,green,blue));  //turn all pixels on
        }
     strip.show();
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
  
void initializeLightTimer() {
  lightTimer = millis();
  setColor((int)RedBrightness,(int)GreenBrightness,(int)BlueBrightness);
 }

 void getColors() {
    //Serial.print("Red:");
    Serial.write(RedBrightness);
    //Serial.print("Green:",DEC);
    Serial.write(GreenBrightness);
    //Serial.print("Blue:");
    Serial.write(BlueBrightness);
  }
 
void setup() {
  //Serial.println("updating");
  // put your setup code here, to run once:
  Serial.begin(9600);
  strip.begin();
  strip.show();
   setColor((int)RedBrightness,(int)GreenBrightness,(int)BlueBrightness);
  
}

void loop() {
 //check for communication from python/Raspberry Pi
// Serial.print("timer");
 //Serial.println(millis() - lightTimer);
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

