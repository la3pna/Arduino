/* SWR ANALYZER CODE.
Based on rotary encoder code from  raybuf, the AD9851 DDS code from Andrew Smallbone, both relased to public domain.
Uses an DDS to generate RF and an wheatstone bridge to read the impedance imbalance.
REF and PWR from 0 to 5V (1024). 

*/

// usually the rotary encoders three pins have the ground pin in the middle
enum PinAssignments {
  encoderPinA = 2,   // rigth
  encoderPinB = 3,   // left
//  clearButton = 28,    // another two pins
  bandbutton = 9
};

//volatile unsigned int frequency = 0;  // a counter for the dial
unsigned int lastReportedPos = 1;   // change management
static boolean rotating=false;      // debounce management
volatile unsigned long frequency;
//int bandno = 1; //band no.
unsigned int band =4 ;

int bandstack[] = { 
  1810, 3500, 5260, 7000, 10100, 14000, 18000, 21000, 24740, 28000, 29000, 50000};

#define FQ_UD 12  // connected to AD9851 device select pin
#define W_CLK 13  // connected to AD9851 clock pin
#define DATA 11   // connected to AD9851 D7 (serial data) pin 
#include <LiquidCrystal.h>
#define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }
LiquidCrystal lcd(10, 8, 7, 6, 5, 4);

// interrupt service routine vars
boolean A_set = false;              
boolean B_set = false;
#define fwdpin 0
#define refpin 1
#define vzpin 2
float fwd; 
float ref;
float vz;
void setup() {

  pinMode(encoderPinA, INPUT); 
  pinMode(encoderPinB, INPUT); 
  //pinMode(clearButton, INPUT);
  pinMode(bandbutton, INPUT);
 // turn on pullup resistors
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);
 // digitalWrite(clearButton, HIGH);
    digitalWrite(bandbutton, HIGH);

// encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);
// encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);

  Serial.begin(9600);  // output
  lcd.begin(16, 2);
  frequency = bandstack[band];
  
    pinMode(FQ_UD, OUTPUT);
  pinMode(W_CLK, OUTPUT);
  pinMode(DATA, OUTPUT);

  // if your board needs it, connect RESET pin and pulse it to reset AD9851
  // pulseHigh(RESET)

  // set serial load enable (Datasheet page 15 Fig. 17) 
  pulseHigh(W_CLK);
  pulseHigh(FQ_UD);
  lcd.print("SWR ANALYZER");
  lcd.setCursor(0, 1);
lcd.print("LA3PNA 2012");
delay (2000);  
}

// main loop, work is done by interrupt service routines, this one only prints stuff
void loop() { 
  rotating = true;  // reset the debouncer

  if ( digitalRead(bandbutton == HIGH) && lastReportedPos != frequency) {
    Serial.print("Index:");
    lastReportedPos = frequency;
    Serial.println((frequency), DEC);
    sendFrequency(frequency);
  }/*
  if (digitalRead(clearButton) == LOW )  {
    delay (150);
    if (digitalRead(clearButton) == LOW )  {
    frequency = 0;
     Serial.println("clear");
     delay(1000);
  }}*/

if (digitalRead(bandbutton) == LOW ) {  
delay(150);   // delay to debounce
 if (digitalRead(bandbutton) == LOW ) {
  band = band + 1;
  if (band > 11) {
    band = 0;
  }
  frequency = bandstack[band];
  delay(1000); //delay to avoid many steps at one
 }  
  Serial.print("band:");
   Serial.println(band);
//  Serial.println(bandstack[band], DEC);
    frequency = bandstack[band];
  }
  
  
  lcd.clear();
    lcd.print("KHz: ");
  lcd.print(frequency);
 
   lcd.setCursor(0, 1);
 lcd.print("SWR: ");
 fwd = analogRead(fwdpin);
ref = analogRead(refpin) ;
 vz = analogRead(vzpin);

 Serial.print("fwd");
 Serial.println(fwd);
  Serial.print("ref");
 Serial.println(ref);

float swr = ((fwd+ref)/(fwd-ref));
float z = ((50*vz)/(fwd-vz));
 //Serial.println(rho);
//  Serial.println(swr);
 lcd.print(swr);

   lcd.setCursor(12, 1);
   lcd.print(z);
 


// lcd.setCursor(12, 0);
 //lcd.print(voltage);
 //Serial.println("voltage");
// Serial.println(svoltage);
 
   delay(30);
 // frequency = frequency + band;
 // Serial.println(frequency, DEC);
//  sendFrequency(frequency)



}
// Interrupt on A changing state
void doEncoderA(){
  // debounce
  if ( rotating ) delay (1);  // wait a little until the bouncing is done

  // Test transition, did things really change? 
  if( digitalRead(encoderPinA) != A_set ) {  // debounce once more
    A_set = !A_set;

    // adjust counter + if A leads B
    if ( A_set && !B_set ) 
     frequency += 5;

    rotating = false;  // no more debouncing until loop() hits again
  }

}

// Interrupt on B changing state, same as A above
void doEncoderB(){
  if ( rotating ) delay (1);
  if( digitalRead(encoderPinB) != B_set ) {
    B_set = !B_set;
    //  adjust counter - 1 if B leads A
    if( B_set && !A_set ) 
      frequency -= 5;

    rotating = false;
  }
}


// transfer a byte a bit at a time LSB first to DATA
void tfr_byte(byte data)
{
  for (int i=0; i<8; i++, data>>=1) {
    digitalWrite(DATA, data & 0x01);
   //   Serial.println(data);
    pulseHigh(W_CLK);
  }
}

// frequency of signwave (datasheet page 12) will be <sys clock> * <frequency tuning word> / 2^32
void sendFrequency(double frequency) {
  int32_t frequencyhz = frequency * 1000;
  Serial.println(frequencyhz);
  int32_t freq = frequencyhz * 4294967296.0 / 180.0e6;
  for (int b=0; b<4; b++, freq>>=8) {
    tfr_byte(freq & 0xFF);
  }
  tfr_byte(0x001);
  pulseHigh(FQ_UD);
}
