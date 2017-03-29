/* Water Level Meter

Measuring water level with ultrasonic sensor HR S04.

Arduino IDE 1.5.8
*/
//red echo
//black trig
int trig = 8;
int echo = 9;
long full_level_raw = 5;
long empty_level_raw = 17;
void setup()
{
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT); 
}

void loop()
{
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
 
 // h = h - 6;  // offset correction
  //h = 50 - h;  // water height, 0 - 50 cm
  //hp = 2 * h;  // distance in %, 0-100 %
  
  // Sending to computer
  Serial.print("raw Reading:");
  Serial.print(h);
  h = h - full_level_raw;
  Serial.print("  adjusted Reading:");
  Serial.print(h);
  h = empty_level_raw - full_level_raw - h;
  Serial.print("  final Reading:");
  Serial.println(h);
    

  delay(1000);
}
