/*
 * Le temperatura usando um sensor LM35
 */

#include "Sensores.h"

LM35::LM35(int pin) {
  pinSensor = pin;
}

int LM35::leTemp () {
  // Seleciona referencia interna do ADC
  analogReference (INTERNAL);
  for (int i = 0; i < 3; i++) {
    analogRead(pinSensor);
    delay(5);
  }
  
  // Le o sensor algumas vezes
  long soma = 0;
  for (int i = 0; i < nAmostras; i++) {
    soma += analogRead(pinSensor);
    delay (10);
  }
  
  long temp = (soma*1100L)/(1024L * nAmostras); 
  return (int) temp;
}

