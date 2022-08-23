#include <LiquidCrystal_I2C.h>

#define pinDiff A1
#define pinDire A2
#define pinPowe A0
#define pinLED  8
#define pinButt 4
int diffusa0 = 0;
int diretta0 = 0;
int diffusa = 0;
int diretta = 0;
float RappDiff = 0;
float RappDire = 0;
float Rapp = 0;
byte Flag = 0;

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  pinMode(pinLED,OUTPUT);
  pinMode(pinButt,INPUT);
  digitalWrite(pinButt,HIGH);
  digitalWrite(pinLED,HIGH);
  delay(1000);
  digitalWrite(pinLED,LOW);
  Serial.begin(9600);
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.clear();
  lcd.print("Turbidity");
  //lcd.print("1234567890123456");
  lcd.setCursor(0,1);
  lcd.print("Push button4Yes");
  delay(5000);
  Serial.println(" Legge Power ");
  int Power = analogRead(pinPowe);
  Serial.println(Power);
//
  lcd.clear();
  lcd.print("Pure water?");
  Serial.println("Pure water?");
  Flag = PushButt(5000);
  Serial.print(" Flag ");Serial.println(Flag);
  delay(1000);
  if(Flag){
    lcd.clear();
    lcd.print("Put pure water");
    Serial.println("Put pure water");
    lcd.setCursor(0,1);
  //lcd.print("1234567890123456");
    lcd.print("When Ready Push");
    Serial.println("When Ready Push");
    byte Rready = PushButt(0);
    Serial.print(Rready);Serial.println(" Ho pigiato ");
    if(Rready){
       digitalWrite(pinLED,HIGH);
       delay(1000);
       diffusa0 = analogRead(pinDiff);
       delay(100);
       diretta0 = analogRead(pinDire);
       delay(100);
       Serial.print("diffusa0 ");Serial.println(diffusa0);
       Serial.print("diretta0 ");Serial.println(diretta0);
       digitalWrite(pinLED,LOW);
       lcd.clear();
       lcd.print("Diff0 ");lcd.print(diffusa0);
       lcd.setCursor(0,1);
       lcd.print("Dire0 ");lcd.print(diretta0);
       delay(5000);
    } 
  }
  lcd.clear();
  lcd.print("Put the sample");
  Serial.println("Put the sample");
  lcd.setCursor(0,1);
  //lcd.print("1234567890123456");
  lcd.print("When Ready Push");
  Serial.println("When Ready Push");
  Flag = PushButt(0);
  Serial.print(" Flag ");Serial.println(Flag);
}
//////////////////////////////////////////////////////////////
void loop() {
  if(Flag){
    digitalWrite(pinLED,HIGH);
    delay(1000);
    diffusa = analogRead(pinDiff);
    delay(100);
    diretta = analogRead(pinDire);
    delay(100);
    Serial.print("Diffusa ");Serial.print(diffusa);Serial.print(" Diretta ");Serial.println(diretta);
    RappDiff = float(diffusa)/float(diffusa0);
    RappDire = float(diretta)/float(diretta0);
    Rapp = RappDiff/RappDire;
    Serial.print("RappDiff ");Serial.print(RappDiff); Serial.print(" RappDire ");Serial.print(RappDire);Serial.print(" Rapp ");Serial.println(Rapp);
    Serial.println("pippo");
    digitalWrite(pinLED,LOW);
    lcd.clear();
    lcd.print("RDif ");lcd.print(RappDiff);
    lcd.setCursor(0,1);
    lcd.print("RDir ");lcd.print(RappDire);
    delay(3000);
  }
}
