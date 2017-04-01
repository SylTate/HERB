int pHupPump = 10;
int pHdownPump = 11;
int nutrientsPump = 12;
int dropPeriod = 200;
char rx_byte = 0;

void setup() {
  pinMode(pHupPump,OUTPUT); 
  pinMode(pHdownPump,OUTPUT); 
  pinMode(nutrientsPump,OUTPUT);
  Serial.begin(9600);
}

void addConcentrate(int pin, int drops) {
  digitalWrite(pin,HIGH);
  delay(drops*dropPeriod);
  digitalWrite(pin,LOW);
  }
  
void loop() {
  
  if (Serial.available() > 0) {    // is a character available?
    rx_byte = Serial.read();       // get the character
  
    // check if a number was received
    if ((rx_byte >= '0') && (rx_byte <= '9')) {
      if (rx_byte == '0') {
          //Serial.print("Number received: ");
         while(Serial.available() <=0) {
          Serial.println("enter number of drops 0-9");
          }
         rx_byte = Serial.read(); 
          Serial.println("pHupPump");
          Serial.print("drops: ");
          Serial.println(rx_byte - '0');
          addConcentrate(pHupPump,rx_byte - '0');
      
      }
      else if (rx_byte == '1') {
        while(Serial.available() <=0) {
          Serial.println("enter number of drops 0-9");
          }
         rx_byte = Serial.read(); 
          Serial.print("pHdown    ");
          Serial.print("drops: ");
           Serial.println(rx_byte - '0');
          addConcentrate(pHdownPump,rx_byte - '0');
      
      }
      else if (rx_byte == '2') {
         while(Serial.available() <=0) {
           Serial.println("enter number of drops 0-9");
          }
          rx_byte = Serial.read(); 
          Serial.print("nutrients    ");
          Serial.print("drops: ");
          Serial.println(rx_byte - '0');
          addConcentrate(nutrientsPump,rx_byte - '0');
        
      }
    }
    else {
      Serial.println("Not a number");
    }
  } // end: if (Serial.available() > 0)

}
