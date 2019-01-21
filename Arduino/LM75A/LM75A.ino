//
// Teste do sensor de temperatura LM75A
//

#include <Wire.h>

// Endereço I2C do sensor
#define ADDR      0x48

// Registradores do sensor
#define REG_TEMP  0
#define REG_CONF  1
#define REG_THYST 2
#define REG_TOS   3

// Iniciação
// Obs: vamos usar a configuração padrão do LM75A
void setup() {
  Serial.begin (9600);
  Wire.begin();

  // Limites para teste
  WriteReg16 (REG_TOS, CodTemp(22.5));
  WriteReg16 (REG_THYST, CodTemp(21.0));
}

// Laço principal, lê e mostra a temperatura
void loop() {
  delay (200);  // dá um tempo para ler
  Serial.println (DecodTemp(ReadReg16(REG_TEMP)));
}

// Rotina para converter temperatura em celsius na
// representação usada no LM75A
int16_t CodTemp (float temp)
{
  return ((int16_t) (temp / 0.125)) << 5; 
}

// Rotina para converter temperatura da
// representação usada no LM75A para celsius
float DecodTemp (int16_t val)
{
  val = val / 32;
  return ((float) val) * 0.125; 
}

// Escreve um valor de 16 bits em um registrador
void WriteReg16 (byte reg, int16_t val)
{
  Wire.beginTransmission(ADDR);
  Wire.write(reg);
  Wire.write((val >> 8) & 0xFF);
  Wire.write(val & 0xFF);
  Wire.endTransmission();
}

// Le um valor de 16 bits de um registrador
int16_t ReadReg16 (byte reg)
{
  uint16_t val;
  
  // Seleciona o registrador
  Wire.beginTransmission(ADDR);
  Wire.write(reg);
  Wire.endTransmission();

  // Faz a leitura
  Wire.requestFrom(ADDR, 2);
  val = Wire.read() << 8;
  val |= Wire.read();
  return (int16_t) val;
}

