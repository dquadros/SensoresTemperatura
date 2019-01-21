/*
 * Teste do sensor DHT11
 * 
 * Daniel Quadros - 31/07/18
 * https://dqsoft.blogspot.com
 */
static const int pinData = 2;
static const int timeout = 200;

// Resposta do sensor
typedef struct
{
  byte umidInt;
  byte umidDec;
  byte tempInt;
  byte tempDec;
  byte checksum;
} RESPOSTA;

// Iniciação
void setup() {
  pinMode (pinData, INPUT);
  Serial.begin (9600);
}

// Laço Principal
void loop() {
  RESPOSTA resp;
  delay (5000);   // ler a cada 5 segundos
  if (leDHT11(&resp)) {
    Serial.print ("Temperatura ");
    Serial.print (resp.tempInt);
    Serial.print (",");
    Serial.print (resp.tempDec);
    Serial.print ("C umidade ");
    Serial.print (resp.umidInt);
    Serial.print (",");
    Serial.print (resp.umidDec);
    Serial.println ("%");
  } else {
    Serial.println ("Falha na leitura");
  }
}

// Efetua a leitura
bool leDHT11 (RESPOSTA *pResp) {
  byte *pDados = (byte *) pResp;
  byte iByte, iBit;
  unsigned long to;

  // Solicita leitura
  pinMode (pinData, OUTPUT);
  digitalWrite (pinData, LOW);
  delay (20);  
  digitalWrite (pinData, LOW);

  // Aguarda confirmar
  pinMode (pinData, INPUT);
  to = micros() + timeout;
  while (digitalRead(pinData) == HIGH) {
    if (micros() > to) {
      return false;
    }
  }
  while (digitalRead(pinData) == LOW) {
    if (micros() > to) {
      return false;
    }
  }
  while (digitalRead(pinData) == HIGH) {
    if (micros() > to) {
      return false;
    }
  }

  // Le os dados
  iByte = iBit = 0;
  while (iByte < 5) {
    // pulso inicial
    to = micros() + timeout;
    while (digitalRead(pinData) == LOW) {
      if (micros() > to) {
        return false;
      }
    }
    // valor do bit
    to = micros() + timeout;
    while (digitalRead(pinData) == HIGH) {
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

