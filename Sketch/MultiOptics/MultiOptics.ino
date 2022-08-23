#include <LiquidCrystal_I2C.h>
#include <FastLED.h>
#define LED_PIN     A1   // pin dati del neopixel
#define NUM_LEDS    1
#define FR90pin     A2   // scatter LDR
#define FR00pin     A0   // direct LDR 
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define pinButt 4
#define pinPowe A3  // eventuale lettura della tensione alimentazione
CRGB leds[NUM_LEDS];

float Rappscat[4];
float RappDire[4];
float Rapp[4];
byte  Flag = 0;
int   direct[4];
int   scattered[4];
int   scattered0[4];
int   direct0[4];
char  *color[]={"RED  ","GREEN","BLUE ","WHITE"};
int   ncicli = 0;
int Bright = 0;

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {

//  set pull-up resistor to pin Button
  pinMode(pinButt,INPUT);
  digitalWrite(pinButt,HIGH);
  delay(10);
  
// set neopixel
  pinMode(LED_PIN,OUTPUT);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
  Serial.begin(9600);
  
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.clear();
  lcd.print("Multi Optics");
  lcd.setCursor(0,1);
  lcd.print("Water Analysis");
  delay(2000);
  lcd.clear();
  lcd.print("Push button for");
  lcd.setCursor(0,1);
  lcd.print("answer YES");
  delay(3000);
  Serial.println(" Legge Power ");
  int Power = analogRead(pinPowe);
  Serial.println(Power);
  Serial.println("Inserire Brightness: 1 - 255");
    Bright = NumeroIntero();
    FastLED.setBrightness( Bright );
    Serial.print(" Brightness ");Serial.println(Bright);

//
  lcd.clear();
  lcd.print("Pure Water");
  lcd.setCursor(0,1);
  lcd.print("Measure?");
  Serial.println("Pure water?");
  Flag = PushButt(5000);
  Serial.print(" Flag ");Serial.println(Flag);
  delay(1000);
  if(Flag){
    lcd.clear();
    lcd.print("Put pure water");
    Serial.println("Put pure water");
    lcd.setCursor(0,1);
    lcd.print("When Ready Push");
    Serial.println("When Ready Push");
    byte Ready = PushButt(0);   // Wait forever
    Serial.print(Ready);Serial.println(" Ho pigiato ");
    if(Ready){
      lcd.clear();
      lcd.print("Wait for measur.");
      ReadSensor(0);
    }
    Serial.println(F("Pure Water Sample"));
    Serial.println(F("Color Scattered direct"));
    for(byte j = 0;j < 4; j++){
      Serial.print(color[j]);Serial.print(F("    "));Serial.print(scattered0[j]);Serial.print(F("    "));Serial.println(direct0[j]);
    }
  }
  lcd.clear();
  lcd.print("Put the sample");
  lcd.setCursor(0,1);
  lcd.print("When Ready Push");
  Serial.println("Put the sample");
  Serial.println("When Ready Push");
  Flag = PushButt(0);  // wait forever
  Serial.print(" Flag ");Serial.println(Flag);
}
//////////////////////////////////////////////////////////////
void loop() {
  if(Flag){
    ncicli++;
    if(ncicli >= 6){
        ncicli = 0;
        Flag = 0;
        lcd.clear();
        lcd.print("Put another s.");
        lcd.setCursor(0,1);
        lcd.print("When Ready Push");
        Serial.println("Put another sample");
        Serial.println("When Ready Push");
        Flag = PushButt(0);  // wait forever
        Serial.print(" Flag ");Serial.println(Flag);
    }
    lcd.clear();
    lcd.print("Wait for measur.");
    ReadSensor(1);
    for(byte j = 0; j < 4; j++){
      if(scattered0[j] > 0){
          Rappscat[j] = float(scattered[j])/float(scattered0[j]);
      }else{
          Rappscat[j] = 999;
      }
      if(direct0[j] > 0){
          RappDire[j] = float(direct[j])/float(direct0[j]);
      }else{
          RappDire[j] = 999;
      }
      if(Rappscat[j] == 999 || RappDire[j] == 999){
            Rapp[j] = 999;
      }else{
            Rapp[j] = Rappscat[j]/RappDire[j];
      }
    }
    Serial.println();
    Serial.println(F("Sample"));
    Serial.println(F("Color Scattered Direct"));
    for(byte j = 0;j < 4; j++){
      Serial.print(color[j]);Serial.print(F("    "));
      Serial.print(scattered[j]);Serial.print(F("    "));
      Serial.println(direct[j]);
    }
    Serial.println(F("Color  RappScat   RappDir    RappScat / RappDir"));
    for(byte j = 0;j < 4; j++){
      Serial.print(color[j]);Serial.print(F("    "));
      Serial.print(Rappscat[j]);Serial.print(F("    "));
      Serial.print(RappDire[j]); Serial.print(F("    "));
      Serial.println(Rapp[j]);
    }

    for(byte j = 0;j < 4; j++){
      lcd.clear();
      lcd.print(color[j]);lcd.print(F(" Rsca "));lcd.print(F(" Rdir"));
      lcd.setCursor(0,1);
      lcd.print(F("n"));lcd.print(ncicli);lcd.print(F("   "));lcd.print(Rappscat[j]);lcd.print(F("  "));lcd.print(RappDire[j]);
      delay(3000);
    }
  }
}
