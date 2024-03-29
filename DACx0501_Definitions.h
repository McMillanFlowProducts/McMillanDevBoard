#ifndef DACx0501_Definitions_H
#define DACx0501_Definitions_H


//DAC I2C ADDRESSES Based on A0
static const uint8_t DAC_ADDR_AGND  = 0x48;
static const uint8_t DAC_ADDR_VCC   = 0x49;
static const uint8_t DAC_ADDR_SDA   = 0x4A;
static const uint8_t DAC_ADDR_SCL   = 0x4B;

//DAC Command Bytes
static const uint8_t DAC_CMD_NOOP    = 0x00;
static const uint8_t DAC_CMD_DEVID   = 0x01;
static const uint8_t DAC_CMD_SYNC    = 0x02;
static const uint8_t DAC_CMD_CONFIG  = 0x03;
static const uint8_t DAC_CMD_GAIN    = 0x04;
static const uint8_t DAC_CMD_TRIGGER = 0x05;
static const uint8_t DAC_CMD_STATUS  = 0x06;
static const uint8_t DAC_CMD_DACn    = 0x08;

//DAC REGISTER Values/Filters
static const uint16_t DAC_REG_REFDIV    = 0x0100;
static const uint16_t DAC_REG_GAIN      = 0x0001;
static const uint16_t DAC_REG_REF_PWDWN = 0x0100;
static const uint16_t DAC_REG_DAC_PWDWN = 0x0001;
static const uint16_t DAC_REG_DAC_SYNC  = 0x0001;
static const uint16_t DAC_REG_RES       = 0x7000;
static const uint16_t DAC_REG_RES12     = 0x2000;
static const uint16_t DAC_REG_RES14     = 0x1000;
static const uint16_t DAC_REG_RES16     = 0x0000;
static const uint16_t DAC_REG_RSTSEL    = 0x0080;
static const uint16_t DAC_REG_LDAC      = 0x0010;
static const uint16_t DAC_REG_SOFT_RES  = 0x000F;
static const uint16_t DAC_REG_REF_ALARM = 0x0001;
static const uint16_t DAC_REG_DAC_DATA  = 0xFFFF;

static const uint8_t DAC_16 = 0;
static const uint8_t DAC_14 = 2;
static const uint8_t DAC_12 = 4;

struct DACx0501Config{
bool REFDIV;
bool GAIN;
bool REF_PWDWN;
bool DAC_PWDWN;
};


#endif