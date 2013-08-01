#include <Adafruit_NeoPixel.h>

#define PIN 6
#define PIX 37
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIX, PIN, NEO_GRB + NEO_KHZ400);
unsigned long INTERVAL = 10000;
unsigned long lastChangeMillis = 0;
unsigned long millisSinceChange = 0;
int lastInput = 0;
int displaySize = 3;
uint32_t bgColor = strip.Color(1,1,1);

int led1min = 0;             
int led1max = 20;
int led2min = 0;
int led2max = 20;
int led3min = 0;
int led3max = 20;
//
float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//
int getValue(long time)
{
  long index = time % INTERVAL;
  float maxVal = (float)INTERVAL; 
  float j = (float)index;
  float rads = fmap(j, 0.0, maxVal, 0.0, 3.14159);
  int nextValue = (int)fmap(sin(rads), 0, 1, led1min, led1max); 
  return nextValue;
}
////
int getValue2(long time)
{
  long index = (time + (INTERVAL/3)) % INTERVAL;
  float maxVal = (float)INTERVAL; 
  float j = (float)index;
  float rads = fmap(j, 0.0, maxVal, 0.0, 3.14159);
  int nextValue = (int)fmap(sin(rads), 0, 1, led2min, led2max); 
  return nextValue;
}

int getValue3(long time)
{
  long index = (time + (2 * (INTERVAL/3))) % INTERVAL;
  float maxVal = (float)INTERVAL; 
  float j = (float)index;
  float rads = fmap(j, 0.0, maxVal, 0.0, 3.14159);
  int nextValue = (int)fmap(sin(rads), 0, 1, led3min, led3max); 
  return nextValue;
}

void showIndex(int index, uint32_t color) {
  for(uint16_t i=0; i < PIX; i++) {
    strip.setPixelColor(i, bgColor);
  }
  for(int j=index; j < index + displaySize; j++) {
    if(j < PIX)
      strip.setPixelColor(j, color);
  }
  strip.show();
}

void setup() {
  Serial.begin(9600);
  strip.begin();
  lastInput = getInputIndex();
  lastChangeMillis = millis();
  strip.show(); // Initialize all pixels to 'off'
}


int getInputIndex() {
  int input1 = analogRead(A0);
  return map(input1, 0, 1023, 0, PIX - 1); 
}

void loop() {
  int now = millis();
 // getValue(now);
  //bgColor = strip.Color(getValue(now),getValue2(now),getValue3(now));
  bgColor = strip.Color(getValue(now),0,0);
  //bgColor = Wheel(map(now % INTERVAL, 0, INTERVAL, 0, 255), 5
  int inputIndex = getInputIndex();
  millisSinceChange = now - lastChangeMillis;
  
  if(inputIndex != lastInput) {
    lastChangeMillis = now;
    lastInput = inputIndex;
  }
  

 // Serial.println(millisSinceChange);
  int maxval = 25;
  int constrainedMillis = constrain(millisSinceChange, 0, maxval);
  int dimness = map(constrainedMillis, 0, maxval, 0, 125);
  //Serial.println(dimness);
  int brightness = 125 - dimness;
  
  int red = brightness;
  int blue = dimness / 2;
  int green = dimness / 2;
  showIndex(inputIndex, strip.Color(red, green, blue));
  delay(1);
}

//// Fill the dots one after the other with a color
//void colorWipe(uint32_t c, uint8_t wait) {
//  for(uint16_t i=0; i<strip.numPixels(); i++) {
//      strip.setPixelColor(i, c);
//      strip.show();
//      delay(wait);
//  }
//}
//
//void colorWipeBack(uint32_t c, uint8_t wait) {
//  for(uint16_t i=0; i<=strip.numPixels(); i++) {
//      strip.setPixelColor(strip.numPixels() - i, c);
//      strip.show();
//      delay(wait);
//  }
//}
//
//void rainbow(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256; j++) {
//    for(i=0; i<strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel((i+j) & 255));
//    }
//    strip.show();
//    delay(wait);
//  }
//}
//
//// Slightly different, this makes the rainbow equally distributed throughout
//void rainbowCycle(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
//    for(i=0; i< strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
//    }
//    strip.show();
//    delay(wait);
//  }
//}
//
//// Input a value 0 to 255 to get a color value.
//// The colours are a transition r - g - b - back to r.

//uint32_t Wheel(byte WheelPos, int maxVal) {
//  if(WheelPos < 85) {
//   return strip.Color(map(WheelPos * 3, 0, 255, 0, maxVal), map(255 - WheelPos * 3, 0, 255, 0, maxVal), 0);
//  } else if(WheelPos < 170) {
//   WheelPos -= 85;
//   return strip.Color(map(255 - WheelPos * 3, 0, 255, 0, maxVal), 0, map(WheelPos * 3, 0, 255, 0, maxVal));
//  } else {
//   WheelPos -= 170;
//   return strip.Color(0, map(WheelPos * 3, 0, 255, 0, maxVal), map(255 - WheelPos * 3, 0, 255, 0, maxVal));
//  }
//}

