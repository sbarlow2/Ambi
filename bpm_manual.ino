//This program calculates BPM and generates the mood profile according to a set of
//ranges.

#include <PulseSensorPlayground.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 3

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

//Declaring colors
uint32_t purple = strip.Color(255, 0, 255);
uint32_t yellow = strip.Color(255, 255, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t orange = strip.Color(255, 165, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t show_color = strip.Color(255, 255, 255); //setting the color initially to white

// Variables for the pulse sensor 
int PulseSensorPurplePin = 10;
float sensorValue = 0;
int count = 9;
unsigned long starttime = 0;
int heartrate = 0;
boolean counted = false;

int Signal;
int Threshold = 500;


// setup() function -- runs once at startup --------------------------------

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  starttime = millis();

//read the pulse sensor for 10 seconds to calculate bpm
  while (millis() < starttime + 10000) {
    sensorValue = analogRead(PulseSensorPurplePin);  // Read the PulseSensor's value. Assign this value to the "Signal" variable.
    if (sensorValue > 550 && counted == false) {
      count++;
      Serial.print("count = ");
      Serial.println(count);
     // colorWipe((show_color), 50);// White, half brightness
     // delay(50);
      counted = true;
    }
    else if (sensorValue < 550) {
      counted = false;
      //colorWipe(strip.Color(250, 250, 250), 50);
    }
  }
//Calculation of BPM taking how many times a pulse is registered in 10 sec
//and then multiplying it by 6 to get the average over a minute. 
  heartrate = count * 6;
  Serial.println();
  Serial.print("BPM = ");
  Serial.println(heartrate);
  Serial.println();
  count = 0;

  if (heartrate >= 90) {   //Purple represents anxious
    show_color = purple;
  }
  else if (heartrate >= 80) { //Orange represents excited
    show_color = orange;
  }
  else if (heartrate >= 70) { //Yellow represents happy
    show_color = yellow;
  }
  else if (heartrate >= 60) { //Green represents calm
    show_color = green;
  }
  else if (heartrate >= 50) { //Blue represents sad
    show_color = blue;
  }
  colorWipe(show_color, 50); //Change the color of the light strip to that color

}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
