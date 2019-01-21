/*
 * Le temperatura usando o sensor LM75A
 */

// Construtor
LM75A::LM75A(byte addr) {
  addrLM75A = addr;
}

// Lê a temperatura
int LM75A::leTemp() {
  // Lê o valor bruto
  uint16_t temp = ReadReg16(REG_TEMP);
  // Retira os bits não usados, alinhando à direita
  temp = temp >> 5;
  // Unidade do sensor é 0.125C, queremos converter para décimo de grau
  temp = (temp*125)/100;
  return temp;
}

// Escreve um valor de 16 bits em um registrador
void LM75A::WriteReg16 (byte reg, int16_t val)
{
  Wire.beginTransmission(addrLM75A);
  Wire.write(reg);
  Wire.write((val >> 8) & 0xFF);
  Wire.write(val & 0xFF);
  Wire.endTransmission();
}

// Le um valor de 16 bits de um registrador
int16_t LM75A::ReadReg16 (byte reg)
{
  uint16_t val;
  
  // Seleciona o registrador
  Wire.beginTransmission(addrLM75A);
  Wire.write(reg);
  Wire.endTransmission();

  // Faz a leitura
  Wire.requestFrom(addrLM75A, 2);
  val = Wire.read() << 8;
  val |= Wire.read();
  return (int16_t) val;
}

