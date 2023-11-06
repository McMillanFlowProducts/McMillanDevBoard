#ifndef MCMILLANSERIAL_H
#define MCMILLANSERIAL_H

#include "DACx0501.h"
#include "MCP3x6x.h"
#include "AD5144A.h"
#include "McMillanSettings.h"

const int BUFFERSIZE = 64;
const char MAXARGS = 4;

class McMillanSerial {
public:
  McMillanSerial(HWCDC *_USB, McMillanSettings *_settings, DACx0501 *_dac, MCP3x6x *_adc, AD5141 *_dpot, bool _multiple = false);
  McMillanSerial(HardwareSerial *_Serial, McMillanSettings *_settings, DACx0501 *_dac, MCP3x6x *_adc, AD5141 *_dpot, bool _multiple = false);
  void begin();
  void begin(int baud);
  void begin(int baud, int rx, int tx);
  void loop();
  void print(const char *_print);
  void printf(const char* format, const char*_print);
  void printf(const char* format, int _print);
  void println(const char *_print);
  void println(int _print);
  HWCDC *USB;
  HardwareSerial *Serial;
protected:
  McMillanSettings *settings;
  DACx0501 *dac;
  MCP3x6x *adc;
  AD5141 *dpot;
  bool factory, multiple, USBMode;
  char buffer[BUFFERSIZE];
  char prevBuffer[BUFFERSIZE];
  uint8_t bufferIndex;
  void command();
  void cmd_set(char *args[]);
  void cmd_get(char *args[]);
  int checkValue(char *value);
  bool strcasecmp(char *string1, const char *string2);
};

#endif  //MCMILLANSERIAL_H