
//
// Teste do sensor de temperatura e humidade HDC1080
//

#include <Wire.h>

// Endereço I2C do sensor
#define SENSOR_ADDR  0x40

// Registradores do sensor
#define HDC1080_TEMP  0x00
#define HDC1080_UMID  0x01
#define HDC1080_CONF  0x02
#define HDC1080_ID0   0xFB
#define HDC1080_ID1   0xFC
#define HDC1080_ID2   0xFD
#define HDC1080_MFG   0xFE
#define HDC1080_DEV   0xFF

// Rotinas locais
static void read_tempumid(uint16_t *pTemp, uint16_t *pUmid);
static uint16_t read_reg(uint8_t reg);
static void write_reg(uint8_t reg, uint16_t val);

// Iniciação
void setup() {
  Serial.begin (9600);
  Wire.begin();

  // Espera terminar o reset
  while (read_reg(HDC1080_CONF) & 0x8000) {
    delay (100);
  }

  // Mostra informações do chip
  Serial.print("Fabricante: ");
  Serial.println(read_reg(HDC1080_MFG), HEX);
  Serial.print("    Modelo: ");
  Serial.println(read_reg(HDC1080_DEV), HEX);

  // Garante a configuração desejada
  write_reg(HDC1080_CONF, 0x1000);
}

// Laco principal
void loop() {
  uint16_t temp, umid;

  // Faz a leitura
  read_tempumid (&temp, &umid);
  
  // Mostra resultado bruto
  Serial.println();
  Serial.println(temp);
  Serial.println(umid);


  // Calcula e mostra a temperatura em celsius
  uint32_t aux;
  aux = temp * 1650L;
  aux = (aux >> 16) - 400L;
  temp = (uint16_t) aux;
  Serial.print(temp/10);
  Serial.print(",");
  Serial.println(temp%10);

  // Calcula e mostra a umidade em %
  aux = umid * 100L;
  aux = aux >> 16;
  umid = (uint16_t) aux;
  Serial.println(umid);

  // Dá um tempo
  delay(10000);
}

// Dispara uma leitura de temperatura e umidade e
// pega o resultado.
static void read_tempumid(uint16_t *pTemp, uint16_t *pUmid) {
  
  // Esta escrita dispara a leitura
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write (HDC1080_TEMP);
  Wire.endTransmission();

  // Aguarda fazer a leitura
  delay(50);

  // Lê o resultado
  if (Wire.requestFrom(SENSOR_ADDR, 4) == 4) {
    *pTemp = Wire.read() << 8;
    *pTemp |= Wire.read();
    *pUmid = Wire.read() << 8;
    *pUmid |= Wire.read();
  } else {
    // algo deu errado
    *pTemp = *pUmid = 0;
  }
}

// Lê um registrador
// Não usar para ler temperatura e umidade!
static uint16_t read_reg(uint8_t reg) {
  uint16_t val;
  
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write (reg);
  Wire.endTransmission();

  if (Wire.requestFrom(SENSOR_ADDR, 2) == 2) {
    val = Wire.read() << 8;
    val |= Wire.read();
  } else {
    val = 0;  // algo deu errado
  }
  return val;
}

// Escreve em um registrador
static void write_reg(uint8_t reg, uint16_t val) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write (reg);
  Wire.write (val >> 8);
  Wire.write (val & 0xFF);
  Wire.endTransmission();
  
}

