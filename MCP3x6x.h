#ifndef MCP3x6x_H
#define MCP3x6x_H

#include <Arduino.h>
#include <SPI.h>
#include "MCP3x6x_Definitions.h"

class MCP3x6x {
public:
  MCP3x6x(int _sclk, int _miso, int _mosi, int _ss, int _spiCLK = 10000000);
  void begin();
//protected:
  int bits, pin_sclk, pin_miso, pin_mosi, pin_ss, spiCLK, value;
  SPIClass *mcp;
  void config();
  void SPIWrite(SPIClass *spi, uint8_t addr, uint32_t data);
  uint32_t SPIRead(SPIClass *spi, uint8_t addr);
  //uint64_t SPI_COMMCRC(_u64data_t CRC_DIVD_64b);
  //void SPI_FASTCMD(SPIClass *spi, uint8_t FAST_CMD);
  //uint8_t RETRIEVE_STATUS(SPIClass *spi);
  //uint8_t CONV_START(uint8_t MUX_CHNL);
};

#endif  //MCP3x6x_H