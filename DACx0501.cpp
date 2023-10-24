#include "DACx0501.h"

DACx0501::DACx0501(uint8_t _address, int _sda, int _scl, int _wireCLK)
  : address(_address), sda(_sda), scl(_scl), wireCLK(_wireCLK) {}

bool DACx0501::begin(DACx0501Config _config) {
  Wire.begin(sda, scl, wireCLK);
  config(_config);
  return getRES();
}

void DACx0501::config(DACx0501Config _config) {
  setREFDIV(_config.REFDIV);
  setGain(_config.GAIN);
  setREF_PWDWN(_config.REF_PWRDWN);
  setDAC_PWDWN(_config.DAC_PWRDWN);
}

bool DACx0501::getRES() {
  switch (read(DAC_CMD_DEVID) & DAC_REG_RES) {
    case DAC_REG_RES12:
      bits = DAC_12;
      break;
    case DAC_REG_RES14:
      bits = DAC_14;
      break;
    case DAC_REG_RES16:
      bits = DAC_16;
      break;
    default:
      return false;
  }
  return true;
}

void DACx0501::setValue(uint16_t _value) {
  if (_value <= (0xFFFF >> bits)){
    write(DAC_CMD_DACn, _value << bits);
  }
  
}

uint16_t DACx0501::getValue() {
  return read(DAC_CMD_DACn) >> bits;
}

void DACx0501::setREFDIV(bool _value) {
  int current = read(DAC_CMD_GAIN);
  if (_value) {
    write(DAC_CMD_GAIN, current | DAC_REG_REFDIV);
  } else {
    write(DAC_CMD_GAIN, current & ~DAC_REG_REFDIV);
  }
}

bool DACx0501::getREFDIV() {
  return (bool)((read(DAC_CMD_GAIN) & DAC_REG_REFDIV) == DAC_REG_REFDIV);
}

void DACx0501::setGain(bool _value) {
  int current = read(DAC_CMD_GAIN);
  if (_value) {
    write(DAC_CMD_GAIN, current | DAC_REG_GAIN);
  } else {
    write(DAC_CMD_GAIN, current & ~DAC_REG_GAIN);
  }
}

bool DACx0501::getGain() {
  return (bool)((read(DAC_CMD_CONFIG) & DAC_REG_GAIN) == DAC_REG_GAIN);
}

void DACx0501::setREF_PWDWN(bool _value) {
  int current = read(DAC_CMD_CONFIG);
  if (_value) {
    write(DAC_CMD_CONFIG, current | DAC_REG_REF_PWDWN);
  } else {
    write(DAC_CMD_CONFIG, current & ~DAC_REG_REF_PWDWN);
  }
}

bool DACx0501::getREF_PWDWN() {
  return (bool)((read(DAC_CMD_CONFIG) & DAC_REG_REF_PWDWN) == DAC_REG_REF_PWDWN);
}

void DACx0501::setDAC_PWDWN(bool _value) {
  int current = read(DAC_CMD_CONFIG);
  if (_value) {
    write(DAC_CMD_CONFIG, current | DAC_REG_DAC_PWDWN);
  } else {
    write(DAC_CMD_CONFIG, current & ~DAC_REG_DAC_PWDWN);
  }
}

bool DACx0501::getDAC_PWDWN() {
  return (bool)((read(DAC_CMD_CONFIG) & DAC_REG_DAC_PWDWN) == DAC_REG_DAC_PWDWN);
}

bool DACx0501::getAlarm() {
  return (bool)((read(DAC_CMD_STATUS) & DAC_REG_REF_ALARM) == DAC_REG_REF_ALARM);
}

uint16_t DACx0501::read(uint8_t cmd) {
  Wire.beginTransmission(address);
  Wire.write(cmd);
  Wire.endTransmission();

  Wire.requestFrom(address, 2);
  uint16_t reply;
  reply = Wire.read() << 8;
  reply |= Wire.read();
  return reply;
}

void DACx0501::write(uint8_t cmd, uint16_t data) {
  Wire.beginTransmission(address);
  Wire.write(cmd);
  Wire.write(data >> 8);
  Wire.write(data | 0xFF);
  Wire.endTransmission();
}