int trig = 8;
int echo = 9;
int submersiblePump = 6;
int solenoidValve = 5;
char rx_byte = 0;

unsigned long valvePulseTime = 500;
byte valveHighV = 170;
byte valveLowV = 85;

void valveOn(int pin) {
  analogWrite(pin, valveHighV);
  delay(valvePulseTime);
  analogWrite(pin, valveLowV);
}

void setup() {
  pinMode(solenoidValve,OUTPUT);
  pinMode(submersiblePump,OUTPUT);
  Serial.begin(9600);
}

void loop() {
   if (Serial.available() > 0) {    // is a character available?
    rx_byte = Serial.read();       // get the character
  
    // check if a number was received
    if ((rx_byte >= '0') && (rx_byte <= '9')) {

      if (rx_byte == '0') {
      //Serial.print("Number received: ");
      analogWrite(submersiblePump, 0);
      Serial.println("pump_off");
      }
      if (rx_byte == '1') {
     // Serial.print("Number received: ");
      Serial.println("pump_on");
      digitalWrite(submersiblePump,HIGH);
      }
      if (rx_byte == '2') {
     // Serial.print("Number received: ");
      Serial.println("valve_0ff");
      analogWrite(solenoidValve,0);
      }
       if (rx_byte == '3') {
        valveOn(solenoidValve);
     // Serial.print("Number received: ");
      Serial.println("valve_on");
      }
      
    }
    else {
      Serial.println("Not a number");
    }
  } // end: if (Serial.available() > 0)

}
