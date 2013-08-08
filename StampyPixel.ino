#include <Adafruit_NeoPixel.h>

#define PIN1 11
#define PIN2 10

#define PIX1 75
#define PIX2 38
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIX1, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(PIX2, PIN2, NEO_GRB + NEO_KHZ800);

int read1Max = 1017;
int read1Min = 19;

unsigned long INTERVAL = 10000;
float fInterval = 10000.0;

unsigned long lastChangeMillis = 0;
unsigned long millisSinceChange = 0;
int lastInput = 0;
int displaySize = 3;
uint32_t bgColor1 = strip.Color(1,1,1);
uint32_t bgColor2 = strip2.Color(1,1,1);

int bgBrightnessFloor = 5;
int bgBrightnessCeiling = 45;

float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float getRadians(float index) {
  Serial.print("getting radians for: ");
  Serial.println(index);
  return fmap(index, 0.0, fInterval, 0.0, 3.14159);
}

int sinMap(float rads) {
   return (int)fmap(sin(rads), 0, 1, bgBrightnessFloor, bgBrightnessCeiling);
}

uint32_t getBgColor(long time)
{
  float rIndex = (float)(time % INTERVAL);
  float gIndex = (float)((time + (INTERVAL/3)) % INTERVAL);
  float bIndex = (float)((time + (2 * (INTERVAL/3))) % INTERVAL);

  float rRads = getRadians(rIndex);
  float gRads = getRadians(gIndex);
  float bRads = getRadians(gIndex);
    
  int r = sinMap(rRads);
  int g = sinMap(gRads);
  int b = sinMap(bRads);
    
  return strip.Color(r,g,b);
}

void showIndex1(int index, uint32_t color) {
  for(uint16_t i=0; i < PIX1; i++) {
    strip.setPixelColor(i, bgColor1);
  }
//  for(int j=index; j < index + displaySize; j++) {
//    if(j < PIX1)
//      strip.setPixelColor(j, color);
//  }
  strip.show();
}

void showIndex2(int index, uint32_t color) {
  for(uint16_t i=0; i < PIX2; i++) {
    strip2.setPixelColor(i, bgColor2);
  }
  for(int j=index; j < index + displaySize; j++) {
    if(j < PIX2)
      strip2.setPixelColor(j, color);
  }
  strip2.show();
}

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip2.begin();
  //lastInput = getInputIndex1();
  lastChangeMillis = millis();
  strip.show(); // Initialize all pixels to 'off'
  //strip2.show();
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipeBack(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  int now = millis();
  bgColor1 = getBgColor(now);
  colorWipeBack(bgColor1, 50);
}


int getInputIndex1() {
  int input1 = analogRead(A0);
  //Serial.println(input1);
  return map(input1, read1Min, read1Max, 0, PIX1 - 1); 
}

int getInputIndex2() {
  int input2 = analogRead(A1);
  return map(input2, 0, 1023, 0, PIX2 - 1); 
}

void loop() {
  int now = millis();
  
  bgColor1 = getBgColor(now);
  bgColor2 = getBgColor(now);
  
  int inputIndex1 = getInputIndex1();
  int inputIndex2 = getInputIndex2();

  millisSinceChange = now - lastChangeMillis;
  
  if(inputIndex1 != lastInput) {
    lastChangeMillis = now;
    lastInput = inputIndex1;
  }
  
  int maxval = 25;
  int constrainedMillis = constrain(millisSinceChange, 0, maxval);
  int dimness = map(constrainedMillis, 0, maxval, 0, 255);
  //Serial.println(dimness);
  int brightness = 255 - dimness;
  
  int red = brightness;
  int blue = dimness / 2;
  int green = dimness / 2;
  showIndex1(0, strip.Color(red, green, blue));
  showIndex2(inputIndex2, strip2.Color(red, green, blue));
  delay(1);
}

//// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
//
void colorWipeBack(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<=strip.numPixels(); i++) {
      strip.setPixelColor(strip.numPixels() - i, c);
      strip.show();
      delay(wait);
  }
}
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

