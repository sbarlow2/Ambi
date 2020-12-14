//This program runs an ambient light cycle simultaneously with the detection of a pulse

#include <Adafruit_NeoPixel.h>

//FIRST STRIP running the ambient light cycle
#define PIXELSPIN   6
#define NUMPIXELS   3
#define CALIBRATIONTIME 20000

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELSPIN, NEO_GRB + NEO_KHZ800);

unsigned long pixelsInterval = 50; // the time we need to wait

unsigned long colorWipePreviousMillis = 0;
unsigned long rainbowPreviousMillis = 0;
unsigned long rainbowCyclesPreviousMillis = 0;

int rainbowCycles = 0;
int rainbowCycleCycles = 0;

uint16_t currentPixel = 0;// what pixel are we operating on

//SECOND STRIP showing the pulse detected
#define HEARTPIN   8
#define HEARTNUMPIXELS   1
//#define CALIBRATIONTIME 20000

Adafruit_NeoPixel heart = Adafruit_NeoPixel(HEARTNUMPIXELS, HEARTPIN, NEO_GRB + NEO_KHZ800);

unsigned long heartInterval = 10; // the time we need to wait

unsigned long hcolorWipePreviousMillis = 0;
unsigned long hrainbowPreviousMillis = 0;
unsigned long hrainbowCyclesPreviousMillis = 0;

int hrainbowCycles = 0;
int hrainbowCycleCycles = 0;

uint16_t hcurrentPixel = 0;// what pixel are we operating on

uint32_t purple = heart.Color(255, 0, 255);
uint32_t yellow = heart.Color(255, 255, 0);
uint32_t green = heart.Color(0, 255, 0);
uint32_t orange = heart.Color(255, 165, 0);
uint32_t blue = heart.Color(0, 0, 255);
uint32_t show_color = heart.Color(255, 255, 255);

// Variables for the pulse sensor
int PulseSensorPurplePin = 10;
float sensorValue = 0;
int count = 9;
unsigned long starttime = 0;
int heartrate = 0;
boolean counted = false;

int Signal;
int Threshold = 560;


void setup() {
  pinMode(PulseSensorPurplePin, INPUT);
  Serial.begin(9600);
  //setting up the ambient light strip
  currentPixel = 0;

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show(); // This sends the updated pixel color to the hardware.

  //setting up the heart strip
  hcurrentPixel = 0;

  heart.begin(); // This initializes the NeoPixel library.
  heart.show(); // This sends the updated pixel color to the hardware.

}

void loop () {

  //running the ambient light cycle without delay() instead checking each
  //loop how much time has elapsed
  if ((unsigned long)(millis() - rainbowPreviousMillis) >= pixelsInterval) {
    rainbowPreviousMillis = millis();
    rainbow();
  }

  sensorValue = analogRead(PulseSensorPurplePin);  // Read the PulseSensor's value. Assign this value to the "Signal" variable.
  if (sensorValue > Threshold) {
    if ((unsigned long)(millis() - hcolorWipePreviousMillis) >= heartInterval) {
      hcolorWipePreviousMillis = millis();
      hcolorWipe(yellow);
//      colorWipe(heart.Color(0, 0, 0));

    }
  }
  else if (sensorValue < Threshold) {
    if ((unsigned long)(millis() - hcolorWipePreviousMillis) >= heartInterval) {
      hcolorWipePreviousMillis = millis();
      hcolorWipe(heart.Color(0, 0, 0));
    }
  }
  //not able to calculate the BPM without causing a delay on the board
  //since we need to check it for ten seconds it pauses the ambient light
  //cycle and interrupts the functions 
  //  heartrate = count * 6;
  //  Serial.println();
  //  Serial.print("BPM = ");
  //  Serial.println(heartrate);
  //  Serial.println();
  //  count = 0;

  if (heartrate >= 90) {
    show_color = purple;
  }
  else if (heartrate >= 80) {
    show_color = orange;
  }
  else if (heartrate >= 70) {
    show_color = yellow;
  }
  else if (heartrate >= 60) {
    show_color = green;
  }
  else if (heartrate >= 50) {
    show_color = blue;
  }
  else {
    show_color = heart.Color(255, 255, 255);
  }

  //  hcolorWipe(show_color);
  //        if ((unsigned long)(millis() - colorWipePreviousMillis) >= pixelsInterval) {
  //          colorWipePreviousMillis = millis();
  //          colorWipe(pixels.Color(0,255,125));
  //        }

  //        if ((unsigned long)(millis() - theaterChasePreviousMillis) >= pixelsInterval) {
  //          theaterChasePreviousMillis = millis();
  //          theaterChase(pixels.Color(127, 127, 127)); // White
  //        }
  //
  //        if ((unsigned long)(millis() - theaterChaseRainbowPreviousMillis) >= pixelsInterval) {
  //          theaterChaseRainbowPreviousMillis = millis();
  //          theaterChaseRainbow();
  //        }

  //        if ((unsigned long)(millis() - rainbowCyclesPreviousMillis) >= pixelsInterval) {
  //            rainbowCyclesPreviousMillis = millis();
  //            rainbowCycle();
  //        }

}
// Fill the dots one after the other with a color
void hcolorWipe(uint32_t c) {
  heart.setPixelColor(hcurrentPixel, c);
  heart.show();
  hcurrentPixel++;
  if (hcurrentPixel == HEARTNUMPIXELS) {
    hcurrentPixel = 0;
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c) {
  pixels.setPixelColor(currentPixel, c);
  pixels.show();
  currentPixel++;
  if (currentPixel == NUMPIXELS) {
    currentPixel = 0;
  }
}

void rainbow() {
  for (uint16_t i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, Wheel((i + rainbowCycles) & 255));
  }
  pixels.show();
  rainbowCycles++;
  if (rainbowCycles >= 256) rainbowCycles = 0;
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle() {
  uint16_t i;

  //for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
  for (i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + rainbowCycleCycles) & 255));
  }
  pixels.show();

  rainbowCycleCycles++;
  if (rainbowCycleCycles >= 256 * 5) rainbowCycleCycles = 0;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
