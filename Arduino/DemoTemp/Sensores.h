/*
 * Classes para interface com os sensores
 */

#ifndef Sensores_h
#define Sensores_h

class Termistor {
  public:
    Termistor (int pin);
    leTemp (void);

  private:
    // Pino onde o sensor está ligado
    int pinSensor;

    // Parâmetros do termistor
    const double beta = 2400.0;
    const double r0 = 1404.0;
    const double t0 = 273.0 + 21.0;
    const double rx = r0 * exp(-beta/t0);
    
    // Parâmetros do circuito
    static const double vcc = 5.0;
    const double R = 2200.0;
    
    // Numero de amostras na leitura
    const int nAmostras = 5;
  
};


class LM35 {
  public:
    LM35 (int pin);
    leTemp (void);

  private:
    // Pino onde o sensor está ligado
    int pinSensor;

    // Numero de amostras na leitura
    const int nAmostras = 5;
};

typedef enum { DHT11, DHT22 } tipoDHT;

class DHT {
  public:
    DHT (tipoDHT tipo, int pin);
    leTemp (void);

  private:
    bool leDHT (void);
    
    // Pino onde o sensor está ligado
    int pinSensor;

    // Tipo do sensor
    tipoDHT tipoSensor;

    // Timeout para resposta do sensor
    const int timeout = 200;

    // Resposta do sensor
    struct
    {
      byte umidInt;
      byte umidDec;
      byte tempInt;
      byte tempDec;
      byte checksum;
    } resposta;
  
};

class LM75A {
  public:
    LM75A (byte addr);
    leTemp (void);

  private:
    // Endereço I2C
    int addrLM75A;

    // Registradores do sensor
    const byte REG_TEMP = 0;
    const byte REG_CONF = 1;
    const byte REG_THYST = 2;
    const byte REG_TOS = 3;
    
    // Métodos auxiliares
    void WriteReg16 (byte reg, int16_t val);
    int16_t ReadReg16 (byte reg);
    
};

class HDC1080 {
  public:
    HDC1080(void);
    int leTemp(void);

  private:
    // Endereço I2C do sensor
    const int SENSOR_ADDR = 0x40;

    // Registradores do sensor
    const uint8_t HDC1080_TEMP  = 0x00;
    const uint8_t HDC1080_UMID  = 0x01;
    const uint8_t HDC1080_CONF  = 0x02;
    const uint8_t HDC1080_ID0   = 0xFB;
    const uint8_t HDC1080_ID1   = 0xFC;
    const uint8_t HDC1080_ID2   = 0xFD;
    const uint8_t HDC1080_MFG   = 0xFE;
    const uint8_t HDC1080_DEV   = 0xFF;

    // Métodos auxiliares
    uint16_t read_reg(uint8_t reg);
    void write_reg(uint8_t reg, uint16_t val);
  
};


#endif

 
