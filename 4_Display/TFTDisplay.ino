/*
  Arduino TFT text example

  This example demonstrates how to draw text on the
  TFT with an Arduino. The Arduino reads the value
  of an analog sensor attached to pin A0, and writes
  the value to the LCD screen, updating every
  quarter second.

  This example code is in the public domain

  Created 15 April 2013 by Scott Fitzgerald
   modified 11 Feb 2015 by Thomas S. Knutsen
  

  http://arduino.cc/en/Tutorial/TFTDisplayText

 */

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>


// pin definition for the Leonardo
 #define cs   7
 #define dc   0
 #define rst  1

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);


void setup() {

  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();

  // clear the screen with a  background
  TFTscreen.background(0, 255, 0);

  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255, 255, 255);
  // set the font size
  TFTscreen.setTextSize(2);
  // write the text to the top left corner of the screen
  TFTscreen.text("LA3PNA 2015 ", 10, 50);
  // ste the font size very large for the loop
  TFTscreen.setTextSize(2);
  delay(2000);
}

void loop() {

  // Clear the screen
  TFTscreen.background(0, 0, 0);
  
  // set the font color
  TFTscreen.stroke(255, 255, 255);
  // print the sensor value

  TFTscreen.text(" some random text ", 10, 50);
  
  // wait for a moment
  delay(2500);
  // erase the text you just wrote
  TFTscreen.background(255, 255, 255);
  
  // set the font color
  TFTscreen.stroke(0, 0, 0);
  // print the sensor value

  TFTscreen.text(" more random text ", 10, 50);
}

