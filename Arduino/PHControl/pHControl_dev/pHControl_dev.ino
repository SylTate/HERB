int pHupPump = 10;
int pHdownPump = 11;
int nutrientsPump = 12;
int dropPeriod = 200;

int target_pH = 7;
void setup() {
   pinMode(pHupPump,OUTPUT); 
   pinMode(pHdownPump,OUTPUT); 
   pinMode(nutrientsPump,OUTPUT);
  // put your setup code here, to run once:
  
  

}
void addConcentrate(int pin, int drops) {
  digitalWrite(pin,HIGH);
  delay(drops*dropPeriod);
  digitalWrite(pin,LOW);
  }

int getPh() {
  return 6;
  }
void phControl() {
  int pH = getPh();
  
  if ( pH < target_pH) {
    addConcentrate(pHupPump, 1);
    
    }
   else if (pH > target_pH) {
    addConcentrate(pHdownPump,1);
    }
  }
void loop() {
  phControl();

}
