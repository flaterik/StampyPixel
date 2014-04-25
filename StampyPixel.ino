#include <StampyPixelLib.h>
//yellow output wire is 9 - bad JST
//red is 10
//black is 11
StampyStrip strip1 = StampyStrip(150, 10, A0, A1, 75, 337, 333, 0); //these are the calibrations for the blue tape
StampyStrip strip2 = StampyStrip(150, 11, A2, A3, 75, 334, 310, 5000); //these are the calbrations for the green tape
//the y cals for both of these is WAY wonky.

void setup() {
  Serial.begin(9600);
  strip1.begin();
  strip2.begin();
  Serial.println("started");
}

void loop() {
  strip1.loop(); 
  strip2.loop();
  delay(1); //I think I want to take this out and go to a timer based approach, but we'll see
}




