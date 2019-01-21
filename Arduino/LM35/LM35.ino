
// Teste do sensor LM35
// Usando referencia interna no ADC

void setup () {
  Serial.begin (38400);
  analogReference (INTERNAL);
}

void loop () {
  int vSensor = analogRead(A1);
  long temp = (vSensor*11000L)/1024L;
  Serial.print (vSensor);
  Serial.print (' ');
  Serial.print (temp/100);
  Serial.print (',');
  Serial.println (temp % 100);
  delay (1000);
}

