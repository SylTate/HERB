
//temperature sensor variables
#include "DHT.h"

#define DHTPIN 2  // temperature sensor to pin 2
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
//LED Variables
byte RedBrightness;
byte GreenBrightness;
byte BlueBrightness;
int redPin = 8;
int greenPin = 9;
int bluePin = 10;
#define COMMON_ANODE
//define variables for the solenoid
int solenoidPin = 5;
int solenoid1State = LOW;
long SolenoidCurrentTime = 0;
//define variables for the pump
int perilstalticPump = 4;
int perilstalticPump1State = LOW;


//define variabled for the submersible pump
int submersiblePump = 3;
int submersiblePumpState = LOW;
long submersiblePumpOffTime = 60000;
unsigned long  submersiblePumpCurrentTime = 0;


//define variables for the waterCycle
unsigned long waterCycleLastMillis = 0;
long waterCycleoffTime = 60000;
int waterCycleState = LOW;

//set variables for ultrasonic sensor
int trig = 12;
int echo = 13;
int maxWaterLevel = 5;
int minWaterLevel = 20;

//define four states for water start from empty state
long fullState = LOW;
long emptyState = HIGH;
long fillingState = LOW;
long emptyingState = LOW;
unsigned long emptyTimer = 0;
unsigned long fullTimer = 0;
long emptyTimerLength = 60000;
long fullTimerLength = 60000;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT); 
  pinMode(solenoidPin,OUTPUT);
  pinMode(perilstalticPump,OUTPUT);
  pinMode(submersiblePump,OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT); 
  dht.begin();

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
void waterCycle() {
  //check the water level
  //turn the pump on for a specific time or until level reached
  //turn on perilstaltic pump as long as water is flowing
  if (getWaterLevel() < maxWaterLevel) {
    digitalWrite(submersiblePump, LOW);
    submersiblePumpState = LOW;
    }
  }
  
void getTemp() {
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
  while (i < 3 && isnan(h)) {
    delay(2000);
    h = dht.readTemperature(true);
    }
   Serial.println(h);
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
  //}

  return;
  }
  void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
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
//check the state of the watercycle and update accordingly
  unsigned long currentMillis = millis();
      //if (submersiblePumpState == HIGH) {
        //if (getWaterLevel() > maxWaterLevel ) {
          // submersiblePumpCurrentTime = currentMillis;
          //}
         //else {
           //digitalWrite(submersiblePump,LOW);
           //submersiblePumpState = LOW;
         //}
      //}
     //else { 
      //if (currentMillis -submersiblePumpCurrentTime > submersiblePumpOffTime ) {
      //digitalWrite(submersiblePump,HIGH);
       //submersiblePumpState = HIGH;
      //} 
      
     //}

     //open and close the solenoid
     //check the states of the system
     if (emptyState == HIGH) {
        Serial.println("Empty State");
        //check the timer
        if (currentMillis - emptyTimer > emptyTimerLength) {
          //transition to next state
          emptyState = LOW;
          fillingState = HIGH;
          //start the pump
          digitalWrite(submersiblePump,HIGH);
          //remember to reset the timer when transitioning to empty state
          }
      }
      else if (fillingState == HIGH) {
        Serial.println("Filling State");
        int waterLevel = getWaterLevel();
        //Serial.print("water level");
        //Serial.println(waterLevel);
        //check the water level
        if (getWaterLevel() < maxWaterLevel) {
          //move to the full state and start full timer
          fillingState = LOW;
          fullState = HIGH;
          fullTimer = currentMillis;
          //stop the pump
          digitalWrite(submersiblePump,LOW);
          }
        }
      else if (fullState == HIGH) {
          //Serial.println("Full State");
          //Serial.print("time left : ");
          //Serial.println((currentMillis-fullTimer) -fullTimerLength);
          if (currentMillis - fullTimer > fullTimerLength) {
            fullState = LOW;
            emptyingState = HIGH;
            digitalWrite(solenoidPin, HIGH);
            }
       }
            //emptying state 
     else if (emptyingState == HIGH) {
        //Serial.println("Emptying State");
        int waterLevel = getWaterLevel();
        //Serial.print("water level");
        //Serial.println(waterLevel);
        if (waterLevel > minWaterLevel) {
          emptyingState = LOW;
          emptyState = HIGH;
          emptyTimer = currentMillis;
          digitalWrite(solenoidPin,LOW);
          }
      }
         

//check for communication from python/Raspberry Pi
if(Serial.available() > 0) {   
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

  setColor((int)RedBrightness,(int)GreenBrightness,(int)BlueBrightness);
  delay(2000);
 

}


