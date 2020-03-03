#include "Arduino.h"
#include <FastLED.h>

#include "Pulse.h"
#include "CycleColors.h"

// Touch pins
#define TOUCH_PIN_ON_OFF 2             // D2
#define TOUCH_PIN_CHANGE_LED_MODES 3   // D3
#define TOUCH_PIN_CYCLE_LED_COLORS 4   // D4

// LED strip constants
#define LED_PIN         6   // D6
#define NUM_LEDS        96
#define MAX_BRIGHTNESS  150
#define NUM_OF_RANDOM_LEDS 5
CRGB leds[NUM_LEDS];

enum ledPattern {
  off,
  pulse,
  cycleColors,
  randomFlash
};

ledPattern ledMode = off;
ledPattern tempLedMode = pulse;
ledPattern standbyLedMode = pulse;

//************************************************************************
// Global Variables
int setupleds = 1;
int delayTime = 20;
int tempDelayTime = 20;
int activeTouchPin = 0;
bool touchPinIsActive = false;
bool uWantToChangeValue = true;

int r = (float)MAX_BRIGHTNESS / 3;
int g = (float)MAX_BRIGHTNESS / 3;
int b = (float)MAX_BRIGHTNESS / 3;
int color[] = {r, g, b};

int ledBrightness = 20;

long randNumber = 0;

int pin0Val = 0;
int pin1Val = 0;
int pin2Val = 0;
//************************************************************************


void setup() 
{
  Serial.begin(9600);

  Serial.println("Setting Up, Please Wait...");

  pinMode(TOUCH_PIN_ON_OFF, INPUT);
  pinMode(TOUCH_PIN_CHANGE_LED_MODES, INPUT);
  pinMode(TOUCH_PIN_CYCLE_LED_COLORS, INPUT);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  ledSetup();
  ledBrightness = MAX_BRIGHTNESS;

  Serial.println("Setup Complete.");
}

void loop()
{
  checkTouchPins();
  if (touchPinIsActive) {
    switch (activeTouchPin) {
      case TOUCH_PIN_ON_OFF:
        if (uWantToChangeValue) {
          if (ledMode == off) {
            ledMode = standbyLedMode;
          } else {
            standbyLedMode = ledMode;
            ledMode = off;
          }
          uWantToChangeValue = false;
        }
        break;
      case TOUCH_PIN_CHANGE_LED_MODES:
        if (uWantToChangeValue) {
          if ((ledMode != off) && (ledMode != cycleColors)) {
          // Cycle ledMode
            if (ledMode == pulse) {
              ledMode = randomFlash;
              randomSeed(analogRead(0)); // Uses random analog noise from pin 0 for randomSeed()
              delayTime = 50;
//              ledBrightness = MAX_BRIGHTNESS;
            } else if (ledMode == randomFlash) {
              ledMode = pulse;
              delayTime = 20;
            }
          }
          uWantToChangeValue = false;
        }
        break;
      case TOUCH_PIN_CYCLE_LED_COLORS:
        if (uWantToChangeValue) {
          if (ledMode != off) {
          // Cycle ledMode
            if (ledMode != cycleColors) {
              tempLedMode = ledMode;
              ledMode = cycleColors;
              
              ledBrightness = (MAX_BRIGHTNESS * 2) / 3;

              tempDelayTime = delayTime;
              delayTime = 20;
            } else {
              ledMode = tempLedMode;
              delayTime = tempDelayTime;
            }
          }
          uWantToChangeValue = false;
        }
        break;
      default:
        break;
    }
  }

  switch (ledMode) {
    case off:
//      Serial.println("ledMode = off");
      setAllLeds(0, 0, 0);
      break;
    case pulse:
      Serial.println("ledMode = pulse");
      ledBrightness = adjustLedBrightness(ledBrightness);
      setAllLeds(r, g, b);
      break;
    case cycleColors:
      Serial.println("ledMode = cycleColors");
      color[0] = r;
      color[1] = g;
      color[2] = b;      
      changeLedColor(color, ledBrightness);
      r = color[0];
      g = color[1];
      b = color[2];
      setAllLeds(r, g, b);
      break;
    case randomFlash:
      Serial.println("ledMode = randomFlash");
      setAllLeds(0, 0, 0);
      for (int i = 0; i < NUM_OF_RANDOM_LEDS; i++) {
        randNumber = random(NUM_LEDS);
        leds[randNumber] = CRGB (r, g, b); // max value is 255
      }
      break;
    default:
      break;
  }

  FastLED.show();

  touchPinIsActive = false;
  delay(delayTime);
}


//************************************************************************
//*** Setup Functions
//************************************************************************

void ledSetup()
{
  /* This for loop sets all of the LEDs to blue in sequence, beginning to end. 
   */  
  for (int i = 0; i < NUM_LEDS; i++) {
    for (int j = 0; j < ledBrightness; j++) {
      leds[i] = CRGB (0, 0, j);  // max value is 255
      FastLED.show();
      delay(40 / (float)ledBrightness);
    }
  }

  while (setupleds >= NUM_LEDS) {
    setupleds -= NUM_LEDS;
  }
  while (setupleds < 0) {
    setupleds += NUM_LEDS;
  }
  setAllLeds(0, 0, ledBrightness);
  leds[setupleds] = CRGB (ledBrightness, 0, ledBrightness);  // max value is 255  

  FastLED.show();
}


//************************************************************************
//*** Running Functions
//************************************************************************
void setAllLeds(int re, int gr, int bl)
{
  re = re * ((float)ledBrightness / 100);
  gr = gr * ((float)ledBrightness / 100);
  bl = bl * ((float)ledBrightness / 100);
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB (re, gr, bl); // max value is 255
  }
}

void checkTouchPins()
{
  pin0Val = (digitalRead(TOUCH_PIN_ON_OFF));
  pin1Val = (digitalRead(TOUCH_PIN_CHANGE_LED_MODES));
  pin2Val = (digitalRead(TOUCH_PIN_CYCLE_LED_COLORS));

  if ((pin0Val == HIGH)
   || (pin1Val == HIGH)
   || (pin2Val == HIGH))
  {
    if (pin0Val == HIGH) {
      touchPinIsActive = true;
      activeTouchPin = TOUCH_PIN_ON_OFF;
      Serial.print("Touch Pin 0: ");

    } else if (pin1Val == HIGH) {
      touchPinIsActive = true;
      activeTouchPin = TOUCH_PIN_CHANGE_LED_MODES;
      Serial.print("Touch Pin 1: ");

    } else if (pin2Val == HIGH) {
      touchPinIsActive = true;
      activeTouchPin = TOUCH_PIN_CYCLE_LED_COLORS;
      Serial.print("Touch Pin 2: ");

    }
  } else {
    touchPinIsActive = false;
    uWantToChangeValue = true;
  }
}
