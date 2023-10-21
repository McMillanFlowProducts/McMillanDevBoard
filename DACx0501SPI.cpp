#include "DACx0501SPI.h"

DACx0501SPI::DACx0501SPI(uint8_t _bits, int _sclk, int _mosi, int _ss, int _spiCLK)
  : bits(_bits), sclk(_sclk), mosi(_mosi), ss(_ss), spiCLK(_spiCLK) {}

void DACx0501SPI::begin() {
  dacspi = new SPIClass(HSPI);
  dacspi->begin(sclk, -1, mosi, ss);  //SCLK - 36, MISO - 37, MOSI - 35, SS - 42
  pinMode(dacspi->pinSS(), OUTPUT);
  digitalWrite(dacspi->pinSS(), HIGH);
  gain = 0x0101;
  config = 0x0000;
  value = 0;

  setREFDIV(1);
  setGain(1);
  set(0xAAFF);
}

void DACx0501SPI::set(int _value) {
  //add code to check range based on dac(16,14,12)
  value = _value;
  spiCommand(dacspi, DAC_CMD_DACn, value << bits);
}

void DACx0501SPI::setREFDIV(bool _value) {
  if (_value) {
    gain |= 0x0100;
  } else {
    gain &= 0x00FF;
  }
  spiCommand(dacspi, DAC_CMD_GAIN, gain);
}

void DACx0501SPI::setGain(bool _value) {
  if (_value) {
    gain |= 0x0001;
  } else {
    gain &= 0xFF00;
  }
  spiCommand(dacspi, DAC_CMD_GAIN, gain);
}

void DACx0501SPI::disableREF(bool _value) {
  if (_value) {
    config |= 0x0100;
  } else {
    config &= 0x00FF;
  }
  spiCommand(dacspi, DAC_CMD_CONFIG, config);
}

void DACx0501SPI::disableDAC(bool _value) {
  if (_value) {
    config |= 0x0001;
  } else {
    config &= 0xFF00;
  }
  spiCommand(dacspi, DAC_CMD_CONFIG, config);
}

void DACx0501SPI::spiCommand(SPIClass *spi, uint8_t cmd, uint16_t data) {
  //use it as you would the regular arduino SPI API
  spi->beginTransaction(SPISettings(spiCLK, MSBFIRST, SPI_MODE3));
  digitalWrite(spi->pinSS(), LOW);  //pull SS slow to prep other end for transfer
  spi->transfer(cmd);
  spi->transfer(data >> 8);
  spi->transfer(data & 0xFF);
  digitalWrite(spi->pinSS(), HIGH);  //pull ss high to signify end of data transfer
  spi->endTransaction();
}