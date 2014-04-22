#include <StampyPixelLib.h>
//vertical:
//Raw Ranges: X: 338-512, Y: 324-512
//338, 324 :: -1.00G, -1.00G
//x down:
//Raw Ranges: X: 325-512, Y: 336-512
//325, 337 :: -1.00G, -0.99G
//
StampyStrip strip1 = StampyStrip(150, 10, A0, A1, 75);
StampyStrip strip2 = StampyStrip(150, 11, A2, A3, 75);
Adafruit_NeoPixel whitestrip = Adafruit_NeoPixel(60, 9,  NEO_GRB + NEO_KHZ800);
//325 : -1, 350, +1 blue tape
//323-348, green tape
//pitch should be atan2(accY, accX) + pi

//const int xInput1 = A0;
//const int yInput1 = A1;
//because we are using a 3.3, actual max is going to be 676, not 1014
int xRawMin = 338;
int xRawMax = 338;
 
int yRawMin = 338;
int yRawMax = 338;
const int sampleSize = 10;

void setup() {
  Serial.begin(9600);
  strip1.begin();
  strip2.begin();
  whitestrip.begin();
  for(uint16_t i=0; i<whitestrip.numPixels(); i++) {
    whitestrip.setPixelColor(i, whitestrip.Color(100,100,255));
  }
  whitestrip.show();
 // sStrip.colorWipeUp(sStrip.getColor(50,50,50), 50);
 Serial.println("started");
}

void loop() {
 //Serial.println("looping");
 strip1.loop(); 
 strip2.loop();
 for(uint16_t i=0; i<whitestrip.numPixels(); i++) {
    whitestrip.setPixelColor(i, whitestrip.Color(100,100,150));
 }
 whitestrip.show();
 delay(1);
  //int xRaw = ReadAxis(xInput1);
  //int yRaw = ReadAxis(yInput1);
 
  //AutoCalibrate(xRaw, yRaw);
  
//  Serial.print("Raw Ranges: X: ");
//    Serial.print(xRawMin);
//    Serial.print("-");
//    Serial.print(xRawMax);
//    Serial.print(", Y: ");
//    Serial.print(yRawMin);
//    Serial.print("-");
//    Serial.print(yRawMax);
//    Serial.println();
//    Serial.print(xRaw);
//    Serial.print(", ");
//    Serial.print(yRaw);
//    
//    // Convert raw values to 'milli-Gs"
//    long xScaled = map(xRaw, xRawMin, xRawMax, -1000, 1000);
//    long yScaled = map(yRaw, yRawMin, yRawMax, -1000, 1000);
//  
//    // re-scale to fractional Gs
//    float xAccel = xScaled / 1000.0;
//    float yAccel = yScaled / 1000.0;
//
//    Serial.print(" :: ");
//    Serial.print(xAccel);
//    Serial.print("G, ");
//    Serial.print(yAccel);
//    Serial.println("G");
//  
//  delay(5000);
}

int ReadAxis(int axisPin) {
  long reading = 0;
  analogRead(axisPin);
  delay(1);
  for (int i = 0; i < sampleSize; i++)
  {
    reading += analogRead(axisPin);
  }
  return reading/sampleSize;
}

void AutoCalibrate(int xRaw, int yRaw)
{
  Serial.println("Calibrate");
  if (xRaw < xRawMin)
  {
    xRawMin = xRaw;
  }
  if (xRaw > xRawMax)
  {
    xRawMax = xRaw;
  }
  
  if (yRaw < yRawMin)
  {
    yRawMin = yRaw;
  }
  if (yRaw > yRawMax)
  {
    yRawMax = yRaw;
  }
 
}


