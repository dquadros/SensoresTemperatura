/*
 * Le temperatura com sensor DHT11 / DHT22
 */

// Construtor
DHT::DHT (tipoDHT tipo, int pin) {
  tipoSensor = tipo;
  pinSensor = pin;
  pinMode (pinSensor, INPUT);
}

// Le temperatura
int DHT::leTemp (void) {
  if (leDHT()) {
    if (tipoSensor == DHT11) {
      return (resposta.tempInt * 10) + resposta.tempDec;
    } else {
      return (resposta.tempInt << 8) + resposta.tempDec;
    }
  } else {
    return 0;
  }
  
}

// Efetua a leitura
bool DHT::leDHT (void) {
  byte *pDados = (byte *) &resposta;
  byte iByte, iBit;
  unsigned long to;

  // Solicita leitura
  pinMode (pinSensor, OUTPUT);
  digitalWrite (pinSensor, LOW);
  delay (20);  

  // Aguarda confirmar
  pinMode (pinSensor, INPUT);
  to = micros() + timeout;
  while (digitalRead(pinSensor) == HIGH) {
    if (micros() > to) {
      return false;
    }
  }
  while (digitalRead(pinSensor) == LOW) {
    if (micros() > to) {
      return false;
    }
  }
  while (digitalRead(pinSensor) == HIGH) {
    if (micros() > to) {
      return false;
    }
  }

  // Le os dados
  iByte = iBit = 0;
  while (iByte < 5) {
    // pulso inicial
    to = micros() + timeout;
    while (digitalRead(pinSensor) == LOW) {
      if (micros() > to) {
        return false;
      }
    }
    // valor do bit
    to = micros() + timeout;
    while (digitalRead(pinSensor) == HIGH) {
      if (micros() > to) {
        return false;
      }
    }
    pDados[iByte] = pDados[iByte] << 1;
    if (((micros() + timeout) - to) > 40) {
      pDados[iByte] |= 1;
    }
    // passa para o próximo bit
    if (++iBit > 7) {
      iBit = 0;
      iByte++;
    }
  }
  
  // Confere o checksum
  return (pDados[0]+pDados[1]+pDados[2]+pDados[3]) == pDados[4];
}


