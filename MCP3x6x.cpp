#include "MCP3x6x.h"

MCP3x6x::MCP3x6x(int _sclk, int _miso, int _mosi, int _ss, int _spiCLK)
  : pin_sclk(_sclk), pin_miso(_miso), pin_mosi(_mosi), pin_ss(_ss), spiCLK(_spiCLK) {}

void MCP3x6x::begin() {
  mcp = new SPIClass(HSPI);
  mcp->begin(pin_sclk, pin_miso, pin_mosi, pin_ss);
  pinMode(mcp->pinSS(), OUTPUT);
  digitalWrite(mcp->pinSS(), HIGH);

  config();
}

void MCP3x6x::SPIWrite(SPIClass* spi, uint8_t addr, uint32_t data) {
  spi->beginTransaction(SPISettings(spiCLK, MSBFIRST, SPI_MODE0));
  digitalWrite(spi->pinSS(), LOW);


  digitalWrite(spi->pinSS(), HIGH);
  spi->endTransaction();
}

uint32_t MCP3x6x::SPIRead(SPIClass* spi, uint8_t addr) {
  spi->beginTransaction(SPISettings(spiCLK, MSBFIRST, SPI_MODE0));

  spi->endTransaction();
  return NULL;
}

void MCP3x6x::config() {
  //GAINCAL --> (8,253,056 / 8,388,607 = 1.615894%) Gain Error w/2.048V Input --> (0b01111011110111010000000 = 0x7DEE80).
  SPIWrite(mcp, _GAINCAL_, GAINCAL_CFG);  // GAINCAL_CFG defined in "MCP3x6x_ADC_Definitions.h" file.

  //OFFSETCAL --> +62 Counts of Offset Cancellation (Measured Offset is Negative) --> (0b000000000000000000000000 = 0x00003E).
  SPIWrite(mcp, _OFFSETCAL_, OFFSETCAL_CFG);  // OFFSETCAL_CFG defined in "MCP3x6x_ADC_Definitions.h" file.

  //TIMER --> Disabled --> (0b000000000000000000000000 = 0x000000).
  SPIWrite(mcp, _TIMER_, TIMER_CFG);  // TIMER_CFG defined in "MCP3x6x_ADC_Definitions.h" file.

  //SCAN --> Disabled --> (0b000000000000000000000000 = 0x000000).
  SPIWrite(mcp, _SCAN_, SCAN_CFG);  // SCAN_CFG defined in "MCP3x6x_ADC_Definitions.h" file.

  //MUX --> VIN+ = CH0, VIN- = CH1 --> (0b00000001 = 0x01).
  SPIWrite(mcp, _MUX_, MUX_CFG);  // MUX_CFG defined in "MCP3x6x_ADC_Definitions.h" file.

  //IRQ --> IRQ Mode = Hi-Z IRQ Output, Fast-CMD = Enabled, Conversion-Start Pulse = Disabled --> (0b00000000 = 0x72).
  SPIWrite(mcp, _IRQ_, IRQ_CFG);  // IRQ_CFG defined in "MCP3x6x_ADC_Definitions.h" file.

  //CONFIG3 --> Conv. Mod = One-Shot Conv. Mode, FORMAT = 24b, CRC_FORMAT = 16b, CRC-COM = Enabled, OFFSETCAL = Enabled, GAINCAL = Enabled --> (0b10000111 = 0x87).
  SPIWrite(mcp, _CONFIG3_, CONFIG3_CFG);  // CONFIG3_CFG Register Configuration defined in "MCP3x6x_ADC_Definitions.h" file.

  //CONFIG2 --> BOOST = 1x, GAIN = 1x, AZ_MUX = ON --> (0b10001111 = 0x8F).
  SPIWrite(mcp, _CONFIG2_, CONFIG2_CFG);  // CONFIG2_CFG defined in "MCP3x6x_ADC_Definitions.h" file.

  //CONFIG1 --> AMCLK = MCLK, OSR = 98304 --> (0b00111100 = 0x3C).
  SPIWrite(mcp, _CONFIG1_, CONFIG1_CFG);  // CONFIG1_CFG defined in "MCP3x6x_ADC_Definitions.h" file.

  //CONFIG0 --> CONFIG[7:6] = Partial Shutdown Mode, CLK_SEL = INTOSC w/o CLKOUT, CS_SEL = No Bias, ADC_MODE = Standby Mode --> (0b11100010 = 0xE2).
  SPIWrite(mcp, _CONFIG0_, CONFIG0_CFG);  // CONFIG0_CFG defined in "MCP3x6x_ADC_Definitions.h" file.

  //LOCK --> 0xA5 enables write-access, LOCK --> 0x00 disables write-access and enables Register Configuration CRC.
  SPIWrite(mcp, _LOCK_, LOCK_CFG);  // LOCK_CFG defined in "MCP3x6x_ADC_Definitions.h" file.
}
