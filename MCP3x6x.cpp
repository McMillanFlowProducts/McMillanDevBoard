#include "MCP3x6x.h"

MCP3x6x::MCP3x6x(int _sclk, int _miso, int _mosi, int _ss, int _spiCLK)
  : sclk(_sclk), miso(_miso), mosi(_mosi), ss(_ss), spiCLK(_spiCLK) {}

void MCP3x6x::begin() {
  mcp = new SPIClass(HSPI);
  mcp->begin(sclk, miso, mosi, spiCLK);
  pinMode(mcp->pinSS(), OUTPUT);
  digitalWrite(mcp->pinSS(), HIGH);

  config();
}

void MCP3x6x::SPIWrite(SPIClass* spi, uint8_t addr, uint32_t REG_CFG) {
  spi->beginTransaction(SPISettings(spiCLK, MSBFIRST, SPI_MODE0));
  //digitalWrite(spi->pinSS(), LOW);

  _u32data_t WR_DATA;  // addr Register write-data variable.

  WR_DATA.DWORD = REG_CFG;  // addr Register write-data value.

  if (addr == _CONFIG0_ || addr == _CONFIG1_ || addr == _CONFIG2_ || addr == _CONFIG3_ || addr == _IRQ_ || addr == _MUX_ || addr == _LOCK_) {
    digitalWrite(spi->pinSS(), LOW);

    spi->transfer((addr << 2) | _WRT_CTRL_);  // Write-CMD of addr Register.
    spi->transfer(WR_DATA.BYTE.low);              // 8-bit addr Register write-data byte.

    digitalWrite(spi->pinSS(), HIGH);
  }
  if (addr == _SCAN_ || addr == _TIMER_ || addr == _OFFSETCAL_ || addr == _GAINCAL_) {
    digitalWrite(spi->pinSS(), LOW);

    spi->transfer((addr << 2) | _WRT_CTRL_);  // Write-CMD of addr Register.
    spi->transfer(WR_DATA.BYTE.upper);            // 24-bit addr Register write-data upper-byte.
    spi->transfer(WR_DATA.BYTE.high);             // 24-bit addr Register write-data high-byte.
    spi->transfer(WR_DATA.BYTE.low);              // 24-bit addr Register write-data low-byte.

    digitalWrite(spi->pinSS(), HIGH);
  }

  spi->endTransaction();
}

uint32_t MCP3x6x::SPIRead(SPIClass* spi, uint8_t addr, _u16data_t& COMM_CRC, uint64_t& CALC_CRC) {
  spi->beginTransaction(SPISettings(spiCLK, MSBFIRST, SPI_MODE0));
  _u32data_t RD_DATA;                                                             // addr Register read-data variable.                                                
    
    _u64data_t CRC_DATA_64b;                                                        
    CRC_DATA_64b.QWORD = 0x0000000000000000;                                        // Reset CRC_DIVD_64b dividend value.

    if (addr == _CONFIG0_ || addr == _CONFIG1_ || addr == _CONFIG2_ || addr == _CONFIG3_ || addr == _IRQ_ || addr == _MUX_ || addr == _RESERVED_C_ || addr == _LOCK_)    
    {
        digitalWrite(spi->pinSS(), LOW);                                             // Assert nCS Pin low.
        
        RD_DATA.BYTE.status = spi->transfer((addr << 2) | _RD_CTRL_);            // Read-CMD of addr Register.
        RD_DATA.BYTE.low = spi->transfer(0x00);                                      // Read 8-bit addr Register data. 
    
        #ifdef ENABLE_SPI_CRC                                                       // Insert SPI Communication-CRC code if enabled. 
            COMM_CRC.BYTE.high = spi->transfer(0x00);                                // Read CRC high-byte.
            COMM_CRC.BYTE.low = spi->transfer(0x00);                                 // Read CRC low-byte.

            CRC_DATA_64b.BYTE.BYTE5 = RD_DATA.BYTE.status;                          // Load STATUS Byte to CRC_DIVD_64b high-high-word high-byte.
            CRC_DATA_64b.BYTE.BYTE4 = RD_DATA.BYTE.low;                             // Load 8-bit DATA-Byte to CRC_DIVD_64b high-high-word low-byte.
        #endif
    
        digitalWrite(spi->pinSS(), HIGH);                                            // Assert nCS Pin high.
    }
    else if (addr == _RESERVED_E_ || addr == _CRCCFG_)
    {            
        digitalWrite(spi->pinSS(), LOW);                                             // Assert nCS Pin low.
        
        RD_DATA.BYTE.status = spi->transfer((addr << 2) | _RD_CTRL_);            // Read-CMD of addr Register.
        RD_DATA.BYTE.high = spi->transfer(0x00);                                     // Read 16-bit addr Register data high-byte.
        RD_DATA.BYTE.low = spi->transfer(0x00);                                      // Read 16-bit addr Register data low-byte.
    
        #ifdef ENABLE_SPI_CRC                                                       // Insert SPI Communication-CRC code if enabled.
            COMM_CRC.BYTE.high = spi->transfer(0x00);                                // Read CRC high-byte.
            COMM_CRC.BYTE.low = spi->transfer(0x00);                                 // Read CRC low-byte.

            CRC_DATA_64b.WORD.upper = RD_DATA.BYTE.status;                          // Load STATUS Byte to CRC_DIVD_64b upper-word.
            CRC_DATA_64b.WORD.highh = RD_DATA.WORD.low;                             // Load 16-bit DATA-Byte to CRC_DIVD_64b high-high-word.
        #endif 
    
        digitalWrite(spi->pinSS(), HIGH);                                            // Assert nCS Pin high. 
    }
    else if (addr == _ADCDATA_ || addr == _SCAN_ || addr == _TIMER_ || addr == _OFFSETCAL_ || addr == _GAINCAL_ || addr == _RESERVED_B_)
    {    
        digitalWrite(spi->pinSS(), LOW);                                             // Assert nCS Pin low.

        RD_DATA.BYTE.status = spi->transfer((addr << 2) | _RD_CTRL_);            // Read-CMD of addr Register. 
        RD_DATA.BYTE.upper = spi->transfer(0x00);                                    // 24-bit addr Register data upper-byte.
        RD_DATA.BYTE.high = spi->transfer(0x00);                                     // 24-bit addr Register data high-byte.
        RD_DATA.BYTE.low = spi->transfer(0x00);                                      // 24-bit addr Register data low-byte.
    
        #ifdef ENABLE_SPI_CRC                                                       // Insert SPI Communication-CRC code if enabled
            COMM_CRC.BYTE.high = spi->transfer(0x00);                                // CRC high-byte.
            COMM_CRC.BYTE.low = spi->transfer(0x00);                                 // CRC low-byte.
      
            CRC_DATA_64b.DWORD.high = RD_DATA.DWORD;                                // Message Data w/STATUS Byte.  
        #endif 
    
        digitalWrite(spi->pinSS(), HIGH);                                            // Assert nCS Pin high.    
    }
    else
    {
        RD_DATA.DWORD = NULL;                                                       // Return NULL Byte if addr is invalid. 
    }
    
    #ifdef ENABLE_SPI_CRC
        CALC_CRC = SPI_COMMCRC(CRC_DATA_64b);                                       // Insert CRC-Calculation if enabled. 
    #endif 

    return RD_DATA.DWORD;                                                           // Return addr Register data. 

  spi->endTransaction();
}

void MCP3x6x::config() {
  //GAINCAL --> (8,253,056 / 8,388,607 = 1.615894%) Gain Error w/2.048V Input --> (0b01111011110111010000000 = 0x7DEE80).
    SPIWrite(mcp, _GAINCAL_, GAINCAL_CFG);                                  // GAINCAL_CFG defined in "MCP3x6x_ADC_Definitions.h" file.                               
    
    //OFFSETCAL --> +62 Counts of Offset Cancellation (Measured Offset is Negative) --> (0b000000000000000000000000 = 0x00003E).
    SPIWrite(mcp, _OFFSETCAL_, OFFSETCAL_CFG);                             // OFFSETCAL_CFG defined in "MCP3x6x_ADC_Definitions.h" file.                                
    
    //TIMER --> Disabled --> (0b000000000000000000000000 = 0x000000).
    SPIWrite(mcp, _TIMER_, TIMER_CFG);                                      // TIMER_CFG defined in "MCP3x6x_ADC_Definitions.h" file.                                     
    
    //SCAN --> Disabled --> (0b000000000000000000000000 = 0x000000).
    SPIWrite(mcp, _SCAN_, SCAN_CFG);                                        // SCAN_CFG defined in "MCP3x6x_ADC_Definitions.h" file.                                      
    
    //MUX --> VIN+ = CH0, VIN- = CH1 --> (0b00000001 = 0x01).
    SPIWrite(mcp, _MUX_, MUX_CFG);                                          // MUX_CFG defined in "MCP3x6x_ADC_Definitions.h" file.                                       
    
    //IRQ --> IRQ Mode = Hi-Z IRQ Output, Fast-CMD = Enabled, Conversion-Start Pulse = Disabled --> (0b00000000 = 0x72).
    SPIWrite(mcp, _IRQ_, IRQ_CFG);                                          // IRQ_CFG defined in "MCP3x6x_ADC_Definitions.h" file.                                       
    
    //CONFIG3 --> Conv. Mod = One-Shot Conv. Mode, FORMAT = 24b, CRC_FORMAT = 16b, CRC-COM = Enabled, OFFSETCAL = Enabled, GAINCAL = Enabled --> (0b10000111 = 0x87).
    SPIWrite(mcp, _CONFIG3_, CONFIG3_CFG);                                  // CONFIG3_CFG Register Configuration defined in "MCP3x6x_ADC_Definitions.h" file.                                   
    
    //CONFIG2 --> BOOST = 1x, GAIN = 1x, AZ_MUX = ON --> (0b10001111 = 0x8F).
    SPIWrite(mcp, _CONFIG2_, CONFIG2_CFG);                                  // CONFIG2_CFG defined in "MCP3x6x_ADC_Definitions.h" file.                                   
    
    //CONFIG1 --> AMCLK = MCLK, OSR = 98304 --> (0b00111100 = 0x3C).
    SPIWrite(mcp, _CONFIG1_, CONFIG1_CFG);                                  // CONFIG1_CFG defined in "MCP3x6x_ADC_Definitions.h" file.                                   
    
    //CONFIG0 --> CONFIG[7:6] = Partial Shutdown Mode, CLK_SEL = INTOSC w/o CLKOUT, CS_SEL = No Bias, ADC_MODE = Standby Mode --> (0b11100010 = 0xE2).
    SPIWrite(mcp, _CONFIG0_, CONFIG0_CFG);                                  // CONFIG0_CFG defined in "MCP3x6x_ADC_Definitions.h" file. 

    //LOCK --> 0xA5 enables write-access, LOCK --> 0x00 disables write-access and enables Register Configuration CRC.
    SPIWrite(mcp, _LOCK_, LOCK_CFG);                                        // LOCK_CFG defined in "MCP3x6x_ADC_Definitions.h" file. 
}

/***********************************************************************************************************************************************************
  Function:
    uint8_t CONV_START(uint8_t)

  Remarks:
    - ADC Conversion-Start Function.   
    - See prototype in MCP3x6x_SPI_Definitions.h.
 */

 uint8_t MCP3x6x::CONV_START(uint8_t MUX_CHNL)
 {
    uint8_t CONV_START_STATUS_BYTE;                                                 // STATUS Byte for CONV_START() function.
    uint16_t STATUS_LOOP_COUNT;                                                     // STATUS Byte retrieval Loop-Count. 
    
    SPIWrite(mcp, _MUX_, MUX_CHNL);                                                       // ADC Conversion on channel MUX_CHNL.  
    
    #ifdef ENABLE_ADCCONV_FASTCMD
      SPI_FASTCMD(mcp, _ADC_CONV_STRST_);                                                // Alternative -- Send ADC-Converstion-Start/Restart Fast-CMD.    
    #else
      SPIWrite(mcp, _CONFIG0_, (CONFIG0_CFG | ADC_MODE_CONV));                            // Write ADC_MODE[1:0] = ADC Conversion Mode.
    #endif

    delay(100);                                                                     // Delay 100ms per MCP3x6x(R) Data-Ready(DR) STATUS Bit Errata.

    STATUS_LOOP_COUNT = 0x00;                                                       // Reset STATUS Byte Loop-Count to 0. 

    while((CONV_START_STATUS_BYTE != 0x13) && STATUS_LOOP_COUNT < 65535)            // Wait until Data-Ready(DR) Bit of STATUS Byte is set.
    {
        CONV_START_STATUS_BYTE = RETRIEVE_STATUS(mcp);                                 // Read STATUS Byte for Data-Ready(DR) Bit.
        STATUS_LOOP_COUNT++;                                                        // STATUS Byte test count limited to 65535. 
    }
    
    return CONV_START_STATUS_BYTE;                                                  // Return STATUS Byte for CONV_START() function. 
 }

/***********************************************************************************************************************************************************
  Function:
    uint8_t RETRIEVE_STATUS(void)

  Remarks:
    - ADC STATUS Byte Retrieval Function.
    - See prototype in MCP3x6x_SPI_Definitions.h.
 */

uint8_t MCP3x6x::RETRIEVE_STATUS(SPIClass *spi)
{
    spi->beginTransaction(SPISettings(spiCLK, MSBFIRST, SPI_MODE0));
    uint8_t RETRIEVE_STATUS_STATUS_BYTE;                                            // STATUS Byte for RETRIEVE_STATUS() function. 

    digitalWrite(spi->pinSS(), LOW);                                                 // Assert nCS Pin low.     

    RETRIEVE_STATUS_STATUS_BYTE = spi->transfer(_RTRV_STATUS_);                      // Retrieve STATUS Byte. 
    
    digitalWrite(spi->pinSS(), HIGH); 
    spi->endTransaction();                                               // Assert nCS Pin high. 

    return RETRIEVE_STATUS_STATUS_BYTE;                                             // Return STATUS Byte for RETRIEVE_STATUS() function. 
}

/***********************************************************************************************************************************************************
  Function:
    void SPI_FASTCMD(uint8_t)

  Remarks:
    - SPI Fast-CMD Function.
    - See prototype in MCP3x6x_SPI_Definitions.h.
 */

void MCP3x6x::SPI_FASTCMD(SPIClass *spi, uint8_t FAST_CMD)
{
    spi->beginTransaction(SPISettings(spiCLK, MSBFIRST, SPI_MODE0));
    digitalWrite(spi->pinSS(), LOW);                                                 // Assert nCS Pin low. 

    spi->transfer(FAST_CMD);                                                         // Send Fast-CMD.                    
    
    digitalWrite(spi->pinSS(), HIGH);                                                // Assert nCS Pin high. 
    spi->endTransaction();
} 

/***********************************************************************************************************************************************************
  Function:
    uint64_t SPI_COMCRC(_u64data_t)

  Remarks:
    - SPI Communication-CRC Algorithm.  
    - See prototype in MCP3x6x_SPI_Definitions.h.
 */

uint64_t MCP3x6x::SPI_COMMCRC(_u64data_t CRC_DIVD_64b)
{
    /****************************************************************************************/
    /*************************** "Appended Zeros" CRC Calculation ***************************/
    /****************************************************************************************/
    
    _uint64_t CRC_POLY_START_64b = 0xC002800000000000;                              // 0x18005 CRC-Polynomial left-justified for 64-bit CRC calculation starting point.
    _uint64_t CRC_POLY_END_64b = 0x00000000C0028000;                                // 0x18005 CRC-Polynomial left-shifted for 64-bit CRC calculation ending point.     
    _uint64_t bitptr_64b = 0x8000000000000000;                                      // Bit-Pointer to find "1's" in the CRC_DVID_64b Dividend value.    

    while(CRC_POLY_START_64b != CRC_POLY_END_64b)
    {
        if((CRC_DIVD_64b.QWORD & bitptr_64b) == bitptr_64b)                         // If Dividend bit is a '1'?
        {
            CRC_DIVD_64b.QWORD = (CRC_DIVD_64b.QWORD ^ CRC_POLY_START_64b);         // Compute XOR for CRC.
        }
        else                                                                        // If Dividend bit is a '0'?
        {   
            CRC_POLY_START_64b = (CRC_POLY_START_64b >> 1);                         // Right-shift polynomial for XOR computation.
            bitptr_64b = (bitptr_64b >> 1);                                         // Right-shift bit-pointer to next bit of Dividend. 
        }
    }

    return CRC_DIVD_64b.QWORD;
} 