/*
 * Le temperatura usando um termistor
 */
 
#include <math.h>
#include "Sensores.h"

Termistor::Termistor(int pin) {
  pinSensor = pin;
}

int Termistor::leTemp(void) {
  // Seleciona VCC como referencia do ADC
  analogReference (DEFAULT);
  for (int i = 0; i < 3; i++) {
    analogRead(pinSensor);
    delay(5);
  }
  
  // Le o sensor algumas vezes
  int soma = 0;
  for (int i = 0; i < nAmostras; i++) {
    soma += analogRead(pinSensor);
    delay (10);
  }

  // Determina a resistência do termistor
  double v = (vcc*soma)/(nAmostras*1024.0);
  double rt = (vcc*R)/v - R;

  // Calcula a temperatura
  double t = (beta / log(rt/rx)) - 273.0;
  return (int) round(t*10.0);
}

