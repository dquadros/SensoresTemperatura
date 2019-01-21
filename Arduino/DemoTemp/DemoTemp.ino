/*
 * Demonstração de sensores de temperatura
 */

#include "Disp0440.h"
#include "Sensores.h"
#include <OneWire.h>
#include <Wire.h>
#include <math.h>

// Conexões dos Sensores
const int pinTermistor = A0;
const int pinLM35 = A1;
const int pinDHT11 = 4;
const int pinDHT22 = 3;
const int pinDS18B20 = 2;

// Conexões do Display
/*
  Vista pela parte inferior
  
  Az  DB6  2  1  DB7  Mr
  Vd  DB4  4  3  DB5  Am
      DB2  6  5  DB3
      DB0  8  7  DB1
  Am  R/W 10  9  E1   Vd
  Lj  V0  12  11 RS   Az
  Vm  VDD 14  13 VSS  Mr
      --  16  15 E2   Rx
*/

const int pinDispE2 = 5;   // Rx
const int pinDispRS = 6;   // Az
const int pinDispE1 = 7;   // Vd
const int pinDispDB7 = 8;  // Mr
const int pinDispDB6 = 9;  // Az
const int pinDispDB5 = 10; // Am
const int pinDispDB4 = 11; // Vd

// Endereço configurado no LM75A
const byte ADDR_LM75A = 0x48;

// Variaveis globais
Disp0440 *disp;
Termistor term(pinTermistor);
LM35 lm35(pinLM35);
DHT dht11(DHT11, pinDHT11);
DHT dht22(DHT22, pinDHT22);
OneWire  ds18b20(pinDS18B20);
bool achouDs18b20;
byte addrDs18b20[8];
LM75A *lm75a;
HDC1080 *hdc1080;

// Iniciação
void setup() {
  Serial.begin(9600);
  disp = new Disp0440(
    pinDispRS, pinDispE1, pinDispE2,
    pinDispDB4, pinDispDB5, pinDispDB6, pinDispDB7
  );
  disp->write("Teste de Sensores de Temperatura");
  disp->setCursor(2,0);
  disp->write("TERM LM35 DHT11 DHT22 DS18B HDC1080 LM75");
  achouDs18b20 = ds18b20.search(addrDs18b20);
  Wire.begin();
  hdc1080 = new HDC1080();
  lm75a = new LM75A(ADDR_LM75A);
}

// Laco eterno (enquanto dure)
void loop() {
  mostra (0, term.leTemp());
  mostra (5, lm35.leTemp());
  mostra (10, dht11.leTemp());
  mostra (16, dht22.leTemp());
  if (achouDs18b20) {
    mostra (22, leTempDS18B20());
  }
  mostra (28, hdc1080->leTemp());
  mostra (36, lm75a->leTemp());
  delay(10000);
}


// Mostra temperatura na tela
// temp é um inteiro de 0 a 999 correspondendo a 0,0 a 99,9
void mostra (int col, int temp) {
  char sTemp[5];

  sTemp[0] = (temp / 100) + '0';
  sTemp[1] = ((temp / 10) % 10) + '0';
  sTemp[2] = ',';
  sTemp[3] = (temp % 10) + '0';
  sTemp[4] = 0;
  disp->setCursor(3,col);
  disp->write(sTemp);
}

int leTempDS18B20() {
  byte data[10];
  
  // Dispara uma conversão
  ds18b20.reset();
  ds18b20.select(addrDs18b20);
  ds18b20.write(0x44);

  // Espera converter
  delay(1000);

  // Lê a resposta
  ds18b20.reset();
  ds18b20.select(addrDs18b20);    
  ds18b20.write(0xBE);
  for (int i = 0; i < 9; i++) {
    data[i] = ds18b20.read();
  }

  // Converte o resultado
  int16_t raw = (data[1] << 8) | data[0];
  byte cfg = (data[4] & 0x60);
  if (cfg == 0x00) raw = raw & ~7;
  else if (cfg == 0x20) raw = raw & ~3;
  else if (cfg == 0x40) raw = raw & ~1;
  float celsius = (float)raw / 1.60;
  return (int) round(celsius);
}

