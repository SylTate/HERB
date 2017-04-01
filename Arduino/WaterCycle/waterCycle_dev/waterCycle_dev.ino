
//set variables for pins

int trig = 8;
int echo = 9;
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(solenoidValve,OUTPUT);
  pinMode(submersiblePump,OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT); 
  resetWaterCycle();

}

void loop() {
  // put your main code here, to run repeatedly:
  waterCycle();
  
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
