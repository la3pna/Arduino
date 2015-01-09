// program to show servo controll with pulse.
//
// Thomas S. Knutsen  9 jan. 20115
// 

int servoPin = 7;
int pulsewith = 1500;
int val;

void setup() {
  // put your setup code here, to run once:
  pinMode(servoPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 180); 
digitalWrite(servoPin, HIGH);
delayMicroseconds(val);
digitalWrite(servoPin,LOW);
delay(20);
}
