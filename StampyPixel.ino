#include <StampyPixelLib.h>

StampyStrip strip1 = StampyStrip(150, 10, A0, 75);
StampyStrip strip2 = StampyStrip(150, 11, A1, 75);

void setup() {
  Serial.begin(9600);
  strip1.begin();
  strip2.begin();
 // sStrip.colorWipeUp(sStrip.getColor(50,50,50), 50);
}

void loop() {
  //Serial.println("looping");
  strip1.loop(); 
  strip2.loop();
  delay(1);
}



