void ReadSensor(byte zero){

// zero = 0  means that zero sample measurement is performed
// zero = 1  means that Turbidity measurement is performed
// zero = 2  means that Orthophosphate measurement is performed
//  Output from LDR releted to red [0], green [1], blue [2], white [3]
    int result_scat[4];  // 90 degree LDR
    int result_dire[4];  // 180 degree LDR

//***************  It always performs all the possible measures*****************
// switch on the RED and read
    leds[0] = CRGB(255, 0, 0);  // Red
    FastLED.show();
    delay(3000);
    result_scat[0]=analogRead(FR90pin);
    delay(10);
    result_dire[0]=analogRead(FR00pin);
    delay(10);
// switch on the GREEN and read
    leds[0] = CRGB(0, 255, 0);  // Green
    FastLED.show();
    delay(3000);
    result_scat[1]=analogRead(FR90pin);
    delay(10);
    result_dire[1]=analogRead(FR00pin);
    delay(10);
// switch on the BLUE and read
    leds[0] = CRGB(0, 0, 255);  // Blue
    FastLED.show();
    delay(3000);
    result_scat[2]=analogRead(FR90pin);
    delay(10);
    result_dire[2]=analogRead(FR00pin);
    delay(10);
 // switch on the WHITE and read
    leds[0] = CRGB(255, 255, 255);  // White
    FastLED.show();
    delay(3000);
    result_scat[3]=analogRead(FR90pin);
    delay(10);
    result_dire[3]=analogRead(FR00pin);
    delay(10);
    leds[0] = CRGB(0, 0, 0);  // White
    FastLED.show();

if(zero == 0){
//  print the results of pure water
    Serial.println(F("Pure Water Sample"));
}else if(zero == 1){
//  print the results of sample of Turbidity
    Serial.println(F("Turbidity sample"));
}else if(zero == 2){
//  print the results of sample of Orthophosphate
    Serial.println(F("Orthophosphate sample"));
}
    Serial.println(F("Color Scattered direct"));
    for(byte j = 0;j < 4; j++){
      Serial.print(color[j]);Serial.print(F("    "));
      Serial.print(result_scat[j]);Serial.print(F("    "));
      Serial.println(result_dire[j]);
    }
    Serial.println();
   
//  It computes the right parameters in the three situations
if(zero == 0){
    for(byte j = 0; j < 4; j++){
      scattered0[j] = result_scat[j];
      direct0[j] = result_dire[j];
    }
}else if(zero == 1){
  if(direct0[3] != 0){
    Rappscat[3] = 10*float(result_scat[3])/float(direct0[3]);
  }else{
    Rappscat[3] = 999.0;
  }
}else if(zero == 2){
   if(direct0[0] > 0){
      if(result_dire[0] > 600){
         Rappscat[0] = -log10(float(result_dire[0])/float(direct0[0]));
      }else{
         Rappscat[0] = 998.0;
      }
   }else{
      Rappscat[0] = 999.0;
   }
}else{
      Serial.println(F("ReadSensor: no valid argument "));
    }
}
