void ReadSensor(byte zero){
    int result_scat[4];
    int result_dire[4];

// switch on the RED and read
    leds[0] = CRGB(255, 0, 0);  // Red
    FastLED.show();
    delay(3000);
    result_scat[0]=analogRead(FR90pin);
    delay(10);
    result_dire[0]=analogRead(FR00pin);
    delay(10);
    /*
    Serial.print(F("RED scat "));Serial.print(result_scat[0]);
    Serial.print(F("RED dire ")); Serial.println(result_dire[0]);
    */
// switch on the GREEN and read
    leds[0] = CRGB(0, 255, 0);  // Green
    FastLED.show();
    delay(3000);
    result_scat[1]=analogRead(FR90pin);
    delay(10);
    result_dire[1]=analogRead(FR00pin);
    delay(10);
    /*
    Serial.print(F("GREEN scat "));Serial.print(result_scat[1]);
    Serial.print(F("GREEN dire ")); Serial.println(result_dire[1]);
    */
// switch on the BLUE and read
    leds[0] = CRGB(0, 0, 255);  // Blue
    FastLED.show();
    delay(3000);
    result_scat[2]=analogRead(FR90pin);
    delay(10);
    result_dire[2]=analogRead(FR00pin);
    delay(10);
    /*
    Serial.print(F("BLUE scat "));Serial.print(result_scat[2]);
    Serial.print(F("BLUE dire ")); Serial.println(result_dire[2]);
   */
// switch on the WHITE and read
    leds[0] = CRGB(255, 255, 255);  // White
    FastLED.show();
    delay(3000);
    result_scat[3]=analogRead(FR90pin);
    delay(10);
    result_dire[3]=analogRead(FR00pin);
    delay(10);
   /*
     Serial.print(F("WHITE scat "));Serial.print(result_scat[3]);
    Serial.print(F("WHITE dire ")); Serial.println(result_dire[3]);
*/
    if(zero == 0){
      for(byte j = 0; j < 4; j++){
        scattered0[j] = result_scat[j];
        direct0[j] = result_dire[j];
      }
      return;
    }else if (zero == 1){
      for(byte j = 0; j < 4; j++){
        scattered[j] = result_scat[j];
        direct[j] = result_dire[j];
      }
      return;
    }else{
      Serial.println(F("ReadSensor: no valid argument "));
    }
}
