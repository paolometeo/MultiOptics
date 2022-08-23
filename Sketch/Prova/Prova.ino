void setup() {
  Serial.begin(9600);

}

void loop() {
 int diretta = analogRead(A0);
 int diffusa = analogRead(A2);
 Serial.print("Diffusa ");Serial.print(diffusa);
 Serial.print(" Diretta ");Serial.println(diretta);
 delay(1000);
}
