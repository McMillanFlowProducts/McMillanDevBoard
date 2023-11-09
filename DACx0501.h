#ifndef DACx0501_H
#define DACx0501_H

#include <Arduino.h>
#include <Wire.h>
#include "DACx0501_Definitions.h"

class DACx0501 {
  public:
    DACx0501(uint8_t address, int _sda, int _scl, int _wireCLK = 10000000);
    bool begin(DACx0501Config _config);
    void config(DACx0501Config _config);
    void setValue(uint16_t _value);
    void setREFDIV(bool _value);
    void setGain(bool _value);
    void setREF_PWDWN(bool _value);
    void setDAC_PWDWN(bool _value);
    uint16_t getValue();
    bool getREFDIV();
    bool getGain();
    bool getREF_PWDWN();
    bool getDAC_PWDWN();
    bool getAlarm();
    bool isConnected();
    ~DACx0501();
  protected:
    int bits, address, sda, scl, wireCLK;
    uint16_t read(uint8_t cmd);
    void write(uint8_t cmd, uint16_t data);
    bool getRES();
};

#endif //DACx0501_H