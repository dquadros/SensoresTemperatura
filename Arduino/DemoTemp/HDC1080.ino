/*
 * Le temperatura usando o sensor HDC1080
 */

// Construtor
HDC1080::HDC1080(void) {
  // Espera terminar o reset
  while (read_reg(HDC1080_CONF) & 0x8000) {
    delay (10);
  }
  
  // Garante a configuração desejada
  write_reg(HDC1080_CONF, 0);
}

// Le a temperatura
int HDC1080::leTemp(void) {
  uint16_t temp;

  // Esta escrita dispara a leitura
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write (HDC1080_TEMP);
  Wire.endTransmission();

  // Aguarda fazer a leitura
  delay(50);

  // Lê o resultado
  if (Wire.requestFrom(SENSOR_ADDR, 2) == 2) {
    temp = Wire.read() << 8;
    temp |= Wire.read();
  } else {
    // algo deu errado
    temp = 0;
  }

  // Converte para decimos de grau
  uint32_t aux;  
  aux = temp * 1650L;  
  aux = (aux >> 16) - 400L;  
  return (uint16_t) aux;
}

// Lê um registrador
// Não usar para ler temperatura e umidade!
uint16_t HDC1080::read_reg(uint8_t reg) {
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
void HDC1080::write_reg(uint8_t reg, uint16_t val) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write (reg);
  Wire.write (val >> 8);
  Wire.write (val & 0xFF);
  Wire.endTransmission();
}


