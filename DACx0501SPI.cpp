#include "DACx0501SPI.h"

DACx0501SPI::DACx0501SPI(int _sclk, int _mosi, int _ss, int _spiCLK) : sclk(_sclk), mosi(_mosi), ss(_ss), spiCLK(_spiCLK){}

void DACx0501SPI::begin(){
  SPIClass *dacspi = new SPIClass(HSPI);
  dacspi->begin(sclk, -1, mosi, ss);  //SCLK - 36, MISO - 37, MOSI - 35, SS - 42
  pinMode(dacspi->pinSS(), OUTPUT);
  digitalWrite(dacspi->pinSS(), HIGH);
  value = 0;

  spiCommand(dacspi, DAC_CMD_SYNC, 0x0000);
  spiCommand(dacspi, DAC_CMD_CONFIG, 0x0000);
  spiCommand(dacspi, DAC_CMD_GAIN, 0x0101);
  spiCommand(dacspi, DAC_CMD_TRIGGER, 0x0000);
  spiCommand(dacspi, DAC_CMD_DACn, 0xAAFF);
}

void DACx0501SPI::set(int _value) {
  //add code to check range based on dac(16,14,12)
  value = _value;
  spiCommand(dacspi, DAC_CMD_DACn, value);
}

void DACx0501SPI::setREFDIV(bool _value){

}

void DACx0501SPI::setGain(bool _value){

}

void DACx0501SPI::disableREF(bool _value){

}

void DACx0501SPI::disableDAC(bool _value){

}

int DACx0501SPI::spiCommand(SPIClass *spi, uint8_t cmd, int data) {
  //use it as you would the regular arduino SPI API
  int reply;
  spi->beginTransaction(SPISettings(spiCLK, MSBFIRST, SPI_MODE3));
  digitalWrite(spi->pinSS(), LOW);  //pull SS slow to prep other end for transfer
  spi->transfer(cmd);
  reply = spi->transfer16(data);
  digitalWrite(spi->pinSS(), HIGH);  //pull ss high to signify end of data transfer
  spi->endTransaction();
  return reply;
}