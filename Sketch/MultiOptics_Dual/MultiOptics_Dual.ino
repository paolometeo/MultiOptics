/*
 * MultiOptics instrument
 * This version is for measuring Turbidity in FTU units and Orthophosphate concentration in ppb
 
 * Calibration was performed with many different FTU samples and Orthophosphate concentration
 * 
 * Hardware:  MCU Arduino UNO; Neopixel LED WS2812B; 2 LDR
 * 
 * Paolo Bonelli
 * 22/07/2022
 */

#include <LiquidCrystal_I2C.h>
#include <FastLED.h>
#define LED_PIN     A1   // pin dati del neopixel
#define NUM_LEDS    1
#define FR90pin     A2   // scatter LDR
#define FR00pin     A0   // direct LDR 
#define pinButt     4
#define pinPowe     A3  // eventuale lettura della tensione alimentazione
#define LED_TYPE    WS2812B
#define BRIGHTNESS  255
CRGB leds[NUM_LEDS];

float Rappscat[4];
float RappDire[4];
byte  waterFlg = 0;
int   scattered0[4];
int   direct0[4];
char  *color[]={"RED  ","GREEN","BLUE ","WHITE"};
int   ncicli = 0;
int   Bright = 0;
byte  turbFlg = 0;
byte  phosFlg = 0;
int   FTU = 0;
int   Phos = 0;

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//////////////////////////////////////////////////////////
void setup() {

//  set pull-up resistor to pin Button
  pinMode(pinButt,INPUT);
  digitalWrite(pinButt,HIGH);
  delay(10);
  
// set neopixel
  pinMode(LED_PIN,OUTPUT);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );

//  set serial monitor
  Serial.begin(9600);

// set LCD display
  lcd.init();                      // initialize the lcd 
  lcd.backlight();

 // presentation
  lcd.clear();
  lcd.print("MultiOpticsDual");
  lcd.setCursor(0,1);
  lcd.print("Water Analysis");
  delay(2000);
  lcd.clear();
  lcd.print("Push button for");
  lcd.setCursor(0,1);
  lcd.print("answer YES");
  delay(3000);

 
// ask what you want to measure  
  Serial.println(" What do you want to measure ? ");
  turbFlg = 0;
  phosFlg = 0;
  lcd.clear();
  lcd.print("Turbidity ?");
  lcd.setCursor(0,1);
  lcd.print("I am waiting 5s");
  turbFlg = PushButt(5000);
  if(turbFlg){
    lcd.setCursor(0,1);
    lcd.print("YES !           ");
    Serial.println(" Turbidity ");
  }else{
    lcd.clear();
    lcd.print("Phosphate ?");
    lcd.setCursor(0,1);
    lcd.print("I am waiting 5s");
    phosFlg = PushButt(5000);
    if(phosFlg){
      lcd.setCursor(0,1);
      lcd.print("YES !           ");
      Serial.println(" Phosphate ");
    }
  }
  if(!turbFlg && !phosFlg){
    lcd.clear();
    lcd.print("No answer!");
    lcd.setCursor(0,1);
    lcd.print("Restart");
    while(1);
  }
  delay(2000);
// ask if you want to measure  pure water
  waterFlg = 0;
  lcd.clear();
  lcd.print("Pure water ?");
  lcd.setCursor(0,1);
  lcd.print("I am waiting 5s");
  Serial.println("Pure water ?");
  waterFlg = PushButt(5000);
  if(waterFlg){
    lcd.setCursor(0,1);
    lcd.print("YES !           ");
    Serial.print(" Flag ");Serial.println(waterFlg);
  }
  delay(1000);

// ask for pure water sample
  if(waterFlg){
    lcd.clear();
    lcd.print("Put pure water");
    lcd.setCursor(0,1);
    lcd.print("when ready push");
    Serial.println("Put pure water,");
    Serial.println("When ready push");
    byte Ready = PushButt(0);   // Wait untill you push the button
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
  }

//  ask for the sample of dirty water
  lcd.clear();
  lcd.print("Put the sample,");
  lcd.setCursor(0,1);
  lcd.print("When ready push");
  Serial.println("Put the sample");
  Serial.println("When ready push");
  waterFlg = PushButt(0);  // wait forever
  Serial.print(" Flag ");Serial.print(waterFlg);Serial.println(" Ho pigiato ");
}
//////////////////////////////////////////////////////////////
void loop() {
  if(waterFlg){
    ncicli++;
//  six measurements of the same sample
//  When finish ask for another sample
    if(ncicli >= 6){   
        ncicli = 0;
        waterFlg = 0;
        lcd.clear();
        lcd.print("Put another s.");
        lcd.setCursor(0,1);
        lcd.print("When ready push");
        Serial.println("Put another sample");
        Serial.println("When ready push");
        waterFlg = PushButt(0);  // wait forever
        Serial.print(" Flag ");Serial.print(waterFlg);Serial.println(" Ho pigiato ");
    }

    lcd.clear();
    lcd.print("Wait for measur.");

//  Measures the sample of dirty water and computes FTU
    if(turbFlg){
      ReadSensor(1);
      float Ra = 0;
      float fFTU = 0;
      if(Rappscat[3] != 999){
        Ra = Rappscat[3];
        fFTU = 3.1302*Ra*Ra - 2.029*Ra + 1.3562;
        FTU = Nint(fFTU);
     }else{
        FTU = 999;
     }
    }
    
//  Measures the sample of dirty water and computes phosphate
   if(phosFlg){
      ReadSensor(2);
      Serial.print(" Rappscat ");Serial.println(Rappscat[0]);
      float Ra = 0;
      float fphos = 0;
      if(Rappscat[0] == 998){   // upper limit
            lcd.clear();
            lcd.print("Orthophosphate");
            lcd.setCursor(0,1);
            lcd.print("ppb > 400");
            Serial.print("Orthophosphate");
            Serial.println(" ppb > 400 ");
            while(1);
      }else if(Rappscat[0] == 999){     // direct0 = 0
            lcd.clear();
            lcd.print("Orthophosphate");
            lcd.setCursor(0,1);
            lcd.print("Wrong measures");
            Serial.print("Orthophosphate");
            Serial.println(" Wrong measures ");
            while(1);        
      }else if(Rappscat[0] < 0.0022){  // lower limit
            lcd.clear();
            lcd.print("Orthophosphate");
            lcd.setCursor(0,1);
            lcd.print("ppb < 10");
            Serial.print("Orthophosphate");
            Serial.println(" ppb < 10 ");
            while(1);
      }else{                                // right measure
            Ra = Rappscat[0];
            fphos = 2652.9*Ra + 3.5784;
            Phos = Nint(fphos);
      }
   }

//  Print results
  if(turbFlg){
    lcd.clear();
    lcd.print("Turbidity");
    lcd.setCursor(0,1);
    lcd.print("FTU: ");lcd.print(FTU);
    Serial.print("Turbidity");
    Serial.print(" FTU: ");Serial.print(FTU);
    
  }
  if(phosFlg){
      lcd.clear();
      lcd.print("Orthophosphate");
      lcd.setCursor(0,1);
      lcd.print("ppb: ");lcd.print(Phos);
      Serial.print("Orthophosphate");
      Serial.print(" ppb: ");Serial.print(Phos);
   }
  }
  delay(3000);
}
