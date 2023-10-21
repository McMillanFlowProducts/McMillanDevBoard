#ifndef DACx0501_H
#define DACx0501_H

#include <Arduino.h>
#include <SPI.h>

//SPIClass *dacspi = NULL;
//static const int spiClk = 10000000;  // 10 MHz
//DAC Command Bytes
static const uint8_t DAC_CMD_NOOP    = 0x00;
static const uint8_t DAC_CMD_DEVID   = 0x01;
static const uint8_t DAC_CMD_SYNC    = 0x02;
static const uint8_t DAC_CMD_CONFIG  = 0x03;
static const uint8_t DAC_CMD_GAIN    = 0x04;
static const uint8_t DAC_CMD_TRIGGER = 0x05;
static const uint8_t DAC_CMD_STATUS  = 0x06;
static const uint8_t DAC_CMD_DACn    = 0x08;

class DACx0501SPI {
  public:
    DACx0501SPI(int _sclk, int _mosi, int _ss, int _spiCLK = 10000000);
    void begin();
    void set(int _value);
    void setREFDIV(bool _value);
    void setGain(bool _value);
    void disableREF(bool _value);
    void disableDAC(bool _value);
  protected:
    int sclk, mosi, ss, spiCLK, value;
    SPIClass *dacspi;
    int spiCommand(SPIClass *spi, uint8_t cmd, int data);

};


#endif //DACx0501_H