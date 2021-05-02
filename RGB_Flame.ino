#include <FastLED.h>

#define DATA_PIN          11
#define CLOCK_PIN         13
#define COLOR_ORDER       BRG
#define CHIPSET           WS2811
#define NUM_LEDS          28
#define NUM_WINDING       4 // NUM_LEDS should be even divisible by NUM_WINDING
#define SECTION           NUM_LEDS / NUM_WINDING

#define BRIGHTNESS        200 // 200
#define FRAMES_PER_SECOND 60

CRGB leds[NUM_LEDS];

void setup() {
  delay(300);
  FastLED.addLeds<LPD6803, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
}

void loop()
{

  fire();
  
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING 70

#define SPREAD_K 0.8


void fire()
{
// Array of temperature readings at each simulation cell
  static byte heat[SECTION];

    for( int i = SECTION - 1; i > 0; i--) {
      heat[i] = heat[i] * SPREAD_K + (1.0 - SPREAD_K) * qsub8( heat[i], random8(0, ((COOLING * 10 * NUM_WINDING) / NUM_LEDS) + 2));
      heat[i] = heat[i] * SPREAD_K + (1.0 - SPREAD_K) * heat[i - 1];
    }
  
    // Set "sparking led"
    heat[0] = heat[0] * SPREAD_K + (1.0 - SPREAD_K) * random8(120, 255);

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < SECTION; j++) {
      CRGB color = HeatColor( heat[SECTION - j - 1]);
      for (int w = 0; w < NUM_WINDING; w++) {
        int idx = (w % 2) == 0 ? j + (w * SECTION) : ((w + 1) * SECTION) - j - 1;
        leds[idx] = color;
      }
    }
}
