
// Programma per comandare qualsiasi colore
//  neopixel di tipo WS2812
#include <FastLED.h>

#define LED_PIN     A1
#define NUM_LEDS    1
#define FR90pin     A2   // Fotoreistor trasversale
#define FR00pin     A0   // Fotoresistore diretto
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B

CRGB leds[NUM_LEDS];

void setup() {
  pinMode(LED_PIN,OUTPUT);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(  0 );
  leds[0] = CRGB(0, 0, 0);  // 
  FastLED.show();
  Serial.begin(9600);
  delay(10000);
  for(int i =0; i < 10; i++){
  Serial.print(analogRead(FR90pin));Serial.print(" "); Serial.println(analogRead(FR00pin));
  }
  delay(100);
  pinMode(LED_PIN,OUTPUT);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
}

void loop() {
  
  leds[0] = CRGB(255, 0, 0);  // Red
  FastLED.show();
  Serial.print(" RED   ");Serial.print(analogRead(FR90pin));Serial.print(" "); Serial.println(analogRead(FR00pin));
  delay(3000);  
  leds[0] = CRGB(0, 255, 0); // Green
  FastLED.show();
   Serial.print(" GREEN ");Serial.print(analogRead(FR90pin));Serial.print(" ");  Serial.println(analogRead(FR00pin));
  delay(3000);
  leds[0] = CRGB(0, 0, 255);  // blue
  FastLED.show();
  Serial.print(" BLUE  "); Serial.print(analogRead(FR90pin));Serial.print(" "); Serial.println(analogRead(FR00pin));
  delay(3000);
  leds[0] = CRGB(255, 255, 255);  // white
  FastLED.show();
  Serial.print(" WHITE ");Serial.print(analogRead(FR90pin));Serial.print(" ");  Serial.println(analogRead(FR00pin));
  delay(3000);
  
}
