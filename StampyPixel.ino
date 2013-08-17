#include <Adafruit_NeoPixel.h>

#define PIN1 11
#define PIN2 10

#define PIX1 75
#define CENTER1 37

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

uint16_t read1Max = 900;
uint16_t read1Min = 100;

unsigned long loopInterval = 10000;
unsigned long loopCount = 0;

unsigned long lastPosition = 0;
unsigned long lastPositionMillis = 0;

int lastInput = 0;
int displaySize = 4;

uint32_t bgColor1 = strip.Color(1,1,1);
uint32_t bgColor2 = strip2.Color(1,1,1);

int bgBrightnessFloor = 0;
int bgBrightnessCeiling = 255;

float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float getRadians(float index, int interval) {
  return fmap(index, 0.0, interval, 0.0, 6.28);
}

int sinMap(float rads) {
   return (int)fmap(sin(rads), -1, 1, bgBrightnessFloor, bgBrightnessCeiling);
}

uint32_t getBgColor() {
  return getLoopedColor(loopCount, loopInterval);
}

uint32_t getLoopedColor(int index, int interval)
{
  float rIndex = (float)(index % interval);
  float gIndex = (float)((index + (interval/3)) % interval);
  float bIndex = (float)((index + (2 * (interval/3))) % interval);

  float rRads = getRadians(rIndex, interval);
  float gRads = getRadians(gIndex, interval);
  float bRads = getRadians(bIndex, interval);
    
  int r = sinMap(rRads);
  int g = sinMap(gRads);
  int b = sinMap(bRads);

  return strip.Color(r,g,b);
}

void showIndex1(int index, uint32_t color) {
  for(uint16_t i=0; i < PIX1; i++) {
    strip.setPixelColor(i, bgColor1);
  }
  for(int j=index; j < index + displaySize; j++) {
    //if(j < PIX1)
     // strip.setPixelColor(j, color);
  }
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
  //strip2.begin();
  unsigned long now = millis();

  strip.show(); // Initialize all pixels to 'off'
  
//  rainbowWipeUp(50);
//  delay(250);
//  colorWipeDown(strip.Color(0,0,0), 50);
//  delay(250);
  bgColor1 = getBgColor();
  colorWipeUp(bgColor1, 50);
  
}

uint16_t getInputPosition1() {
  uint16_t readValue = (uint16_t) analogRead(A0);
//  
//  if(readValue < read1Min) {
//    Serial.println("Setting read 1 min");
//    Serial.println(readValue);
//    //read1Min = readValue;
//  }
//  if(readValue > read1Max) {
//    Serial.println("Setting read 1 max");
//    Serial.println(readValue);
//    read1Max = readValue;
//  }
  readValue;
}

//uint16_t getInputIndex1() {
//  int input1 = getInputPosition1();
// 
//  //Serial.println(input1);
//  return map(input1, read1Min, read1Max, 0, PIX1 - 1); 
//}

int getInputIndex2() {
  int input2 = analogRead(A1);
  return map(input2, 0, 1023, 0, PIX2 - 1); 
}

const int sampleBufferSize = 50;
uint16_t sampleBuffer[sampleBufferSize];
uint8_t sampleIndex = 0;
uint32_t maxBufferSum = sampleBufferSize * 1023;
uint32_t minBufferSum = 0;

void loop() {
  loopCount++;
  if(loopCount > loopInterval) loopCount = 0; //this should be taken care of by the loop color function but there were some occasional weird jumps. there's no way a long was rolling over, but this seems to fix it. 
  
  int now = millis();
  //Serial.println(now - lastPositionMillis);
  bgColor1 = getBgColor();
 // bgColor2 = getBgColor(now);
  
  int inputPosition1 = getInputPosition1();

  if(sampleIndex >= sampleBufferSize - 1) {
    uint32_t sampleSum = 0;
    //Serial.println("reading sample buffer");
    for(int i = 0 ; i < sampleBufferSize ; i++) {
      sampleSum += sampleBuffer[i]; 
      //Serial.println(sampleBuffer[i]);
      sampleBuffer[i] = 0;
    }
    uint32_t averaged = sampleSum / sampleBufferSize;
      if(averaged < read1Min) {
        read1Min = averaged;
      }
      if(averaged > read1Max) {
        read1Max = averaged;
      }
    Serial.println(map(averaged, read1Min, read1Max, 0, PIX1 - 1));
    //Serial.println(sampleSum / sampleBufferSize);
    sampleIndex = 0;
  }
  sampleBuffer[sampleIndex++] = inputPosition1;
  
 // int inputIndex1 = getInputIndex1();
  //int inputIndex2 = getInputIndex2();

//  if(loopCount % 3 == 0) {
//    int time1 = (int) now - lastPositionMillis;
//    //Serial.println(time1);
//    int distance1 = abs(lastPosition - inputPosition1);
//    lastPosition = inputPosition1;
//    if(distance1 <= 15) {
//      distance1 = 0;
//    }
//    if(distance1 != 0) Serial.println(distance1);
//    float velocity = abs((float)distance1 / (float)time1);
    lastPositionMillis = now;
//  }
//  
  //if(velocity != 0) Serial.println(velocity);
  
  
  
  int red = 125;
  int blue = 125;
  int green = 125;
  
  showIndex1(0, strip.Color(red, green, blue));
  //showIndex2(inputIndex2, strip2.Color(red, green, blue));
  delay(1);
}

void colorWipeDown(uint32_t c, uint8_t wait) {
  int8_t i, j;
  for(i = CENTER1, j = CENTER1 + 1; i >= 0; i--, j++) {
    strip.setPixelColor(i, c);
    if(j < PIX1) strip.setPixelColor(j, c);
    strip.show();
    delay(wait);
  }
}

void colorWipeUp(uint32_t c, uint8_t wait) {
  int8_t i, j;
  for(i = 0, j = PIX1 - 1; i < CENTER1 ; i++, j--) {
    strip.setPixelColor(i, c);
    strip.setPixelColor(j, c);
    strip.show();
    delay(wait);
  }
}

void rainbowWipeDown(uint8_t wait) {
  int8_t i, j;
  for(i = CENTER1, j = CENTER1 + 1; i >= 0; i--, j++) {
    strip.setPixelColor(i, getLoopedColor(i, PIX1));
    if(j < PIX1) strip.setPixelColor(j, getLoopedColor(j, PIX1));
    strip.show();
    delay(wait);
  }
}

void rainbowWipeUp(uint8_t wait) {
  int8_t i, j;
  for(i = 0, j = PIX1 - 1; i < CENTER1 ; i++, j--) {
    strip.setPixelColor(i, getLoopedColor(i, PIX1));
    strip.setPixelColor(j, getLoopedColor(j, PIX1));
    strip.show();
    delay(wait);
  }
}

