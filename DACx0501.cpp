#include "DACx0501.h"

DACx0501::DACx0501(uint8_t _bits, int _address, int _sda, int _scl, int _wireCLK)
  : bits(_bits), address(_address), sda(_sda), scl(_scl), wireCLK(_wireCLK) {}

void DACx0501::begin() {
  Wire.begin(sda, scl, wireCLK);

  setREFDIV(true);
  setGain(true);
  setValue(0xAAFF);
}

void DACx0501::setValue(int _value) {
  write(DAC_CMD_DACn, _value);
}

uint16_t DACx0501::getValue() {
  return read(DAC_CMD_DACn);
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