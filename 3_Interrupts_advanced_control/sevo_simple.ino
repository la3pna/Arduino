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
 
digitalWrite(servoPin, HIGH);
delayMicroseconds(pulsewith);
digitalWrite(servoPin,LOW);
delay(20);
}
