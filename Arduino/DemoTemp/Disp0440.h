/*
 * Controle de display de 4 linhas por 40 colunas
 */

#ifndef Disp0440_h
#define Disp0440_h

#include <inttypes.h>

class Disp0440 {

  public:
    Disp0440(byte rs, byte en1, byte en2,
          byte d0, byte d1, byte d2, byte d3);

    void clear(void);
    void setCursor(byte line, byte col); 
    void cursorOn(void);
    void cursorOff(void);
    void write(byte car);
    void write(char *str);

  private:
    void setRS(byte valor);
    void setE(byte en, byte valor);
    void setData(byte nib);
    void writeByte(byte en, byte rs, byte data);
    void writeCmd(byte en, byte cmd);
    void writeChar(byte en, byte chr);
    void rollUp(void);
    void init(byte en);

    const byte CMD = LOW;
    const byte DATA = HIGH;
    
    const byte CMD_CLS = 0x01;
    const byte CMD_DISPON_CURON = 0x0E;
    const byte CMD_DISPON_NOCUR = 0x0C;
    const byte CMD_POSCUR = 0x80;
    const byte CMD_FUNCTIONSET = 0x20;
    const byte LCD_4BITMODE = 0x00;
    const byte LCD_2LINE = 0x08;
    const byte LCD_5x8DOTS = 0x00;

    byte pinRS, pinEN1, pinEN2;
    byte pinD0, pinD1, pinD2, pinD3;

    byte linAtl, colAtl, enAtl;
    byte curOn;
    byte screen[160];
};


#endif

