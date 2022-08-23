/*
 * MultiOptics instrument
 * This version is for measuring Turbidity in FTU units
 * It uses white light
 * Calibration was performed with many different FTU samples
 * 
 * Hardware:  MCU Arduino UNO; Neopixel LED WS2812B; 2 LDR
 * 
 * Paolo Bonelli
 * 05/06/2022
 */

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

//  set serial monitor
  Serial.begin(9600);

// set LCD display
  lcd.init();                      // initialize the lcd 
  lcd.backlight();

 // presentation
  lcd.clear();
  lcd.print("Multi Optics FTU");
  lcd.setCursor(0,1);
  lcd.print("Water Analysis");
  delay(2000);
  lcd.clear();
  lcd.print("Push button for");
  lcd.setCursor(0,1);
  lcd.print("answer YES");
  delay(3000);
  /*
  Serial.println(" Legge Power ");
  int Power = analogRead(pinPowe);
  Serial.println(Power);
  Serial.println("Inserire Brightness: 1 - 255");
    Bright = NumeroIntero();
    FastLED.setBrightness( Bright );
    Serial.print(" Brightness ");Serial.println(Bright);
*/
// set LED brightness 
  Bright = 255;
  
// ask if you want to measure  pure water
  lcd.clear();
  lcd.print("Pure water");
  lcd.setCursor(0,1);
  lcd.print("measure");
  Serial.println("Pure water");
  Flag = 1;
  Serial.print(" Flag ");Serial.println(Flag);
  delay(1000);

// ask for pure water sample
  if(Flag){
    lcd.clear();
    lcd.print("Put pure water");
    lcd.setCursor(0,1);
    lcd.print("when ready push");
    Serial.println("Put pure water,");
    Serial.println("When ready push");
    byte Ready = PushButt(0);   // Wait forever
    Serial.print(Ready);Serial.println(" Ho pigiato ");

//  measure the pure water sample
    if(Ready){
      lcd.clear();
      lcd.print("Wait for measur.");
      ReadSensor(0);
      lcd.clear();
      lcd.print("R ");lcd.print(direct0[0]);lcd.print(" G ");lcd.print(direct0[1]);
      lcd.setCursor(0,1);
      lcd.print("B ");lcd.print(direct0[2]);lcd.print(" W ");lcd.print(direct0[3]);
      delay(4000);
    }
    
//  print the results of pure water
    Serial.println(F("Pure Water Sample"));
    Serial.println(F("Color Scattered direct"));
    for(byte j = 0;j < 4; j++){
      Serial.print(color[j]);Serial.print(F("    "));
      Serial.print(scattered0[j]);Serial.print(F("    "));
      Serial.println(direct0[j]);
    }
  }

//  ask for the sample of dirty water
  lcd.clear();
  lcd.print("Put the sample,");
  lcd.setCursor(0,1);
  lcd.print("When ready push");
  Serial.println("Put the sample");
  Serial.println("When ready push");
  Flag = PushButt(0);  // wait forever
  Serial.print(" Flag ");Serial.print(Flag);Serial.println(" Ho pigiato ");
}
//////////////////////////////////////////////////////////////
void loop() {
  if(Flag){
    ncicli++;
//  six measurements of the same sample
//  When finish ask for another sample
    if(ncicli >= 6){   
        ncicli = 0;
        Flag = 0;
        lcd.clear();
        lcd.print("Put another s.");
        lcd.setCursor(0,1);
        lcd.print("When ready push");
        Serial.println("Put another sample");
        Serial.println("When ready push");
        Flag = PushButt(0);  // wait forever
        Serial.print(" Flag ");Serial.print(Flag);Serial.println(" Ho pigiato ");
    }

//  measure the sample of dirty water
    lcd.clear();
    lcd.print("Wait for measur.");
    ReadSensor(1);
    
//  Print on PC
     Serial.println();
     Serial.println(F("Sample"));
     Serial.print(color[0]);Serial.print(" ");Serial.print(scattered[0]);Serial.print(" ");Serial.println(direct[0]);
     Serial.print(color[1]);Serial.print(" ");Serial.print(scattered[1]);Serial.print(" ");Serial.println(direct[1]);
     Serial.print(color[2]);Serial.print(" ");Serial.print(scattered[2]);Serial.print(" ");Serial.println(direct[2]);
     Serial.print(color[3]);Serial.print(" ");Serial.print(scattered[3]);Serial.print(" ");Serial.println(direct[3]);
     Serial.println();
     
//  compute the R factors
    for(byte j = 0; j < 4; j++){
      if(direct0[j] > 0){
          Rappscat[j] = 10*float(scattered[j])/float(direct0[j]);
      }else{
          Rappscat[j] = 999;
      }
    }
    
//  Print results
     lcd.clear();
     lcd.print(color[0]);lcd.print(" ");lcd.print(Rappscat[0],2);
     lcd.setCursor(0,1);
     lcd.print(color[1]);lcd.print(" ");lcd.print(Rappscat[1],2);
     delay(3000);
     lcd.clear();
     lcd.print(color[2]);lcd.print(" ");lcd.print(Rappscat[2],2);
     lcd.setCursor(0,1);
     lcd.print(color[3]);lcd.print(" ");lcd.print(Rappscat[3],2);
     delay(3000);

//  compute FTU by means of WHITE light. Second degree polynomial Calib20220531-GT.xlsx
     float Ra = 0;
     int FTU = 0;
     float fFTU = 0;
     if(Rappscat[3] != 999){
        Ra = Rappscat[3];
        fFTU = 3.1302*Ra*Ra - 2.029*Ra + 1.3562;
        FTU = Nint(fFTU);
     }else{
        FTU = 999;
     }

 // print FTU
     lcd.clear();
     lcd.print("FTU ");lcd.print(FTU);
     delay(3000);

//  Print on PC
     Serial.println();
     Serial.println(F("Sample"));
     Serial.print(color[0]);Serial.print(" ");Serial.print(Rappscat[0],2);Serial.print(" ");
     Serial.print(color[1]);Serial.print(" ");Serial.print(Rappscat[1],2);Serial.print(" ");
     Serial.print(color[2]);Serial.print(" ");Serial.print(Rappscat[2],2);Serial.print(" ");
     Serial.print(color[3]);Serial.print(" ");Serial.print(Rappscat[3],2);
     Serial.println();
     Serial.print(" FTU ");Serial.println(FTU);
  }
}
