      int  NumeroIntero(){
         int num1 = 0;
         Serial.println(F(" Scrivi tre numeri interi da -32767 a + 32767 separati dalla virgola"));
         while (Serial.available() == 0){     
         }
         while(Serial.available() > 0){
            num1 = Serial.parseInt();
             if(Serial.read() == '\n'){
               break;
            }
         }
         return num1;    
      }
