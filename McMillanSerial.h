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
  McMillanSerial(McMillanSettings *_settings, DACx0501 *_dac, MCP3x6x *_adc, AD5141 *_dpot);
  void begin();
  void loop();
  bool strcasecmp(char *string1, char *string2);
protected:
  McMillanSettings *settings;
  DACx0501 *dac;
  MCP3x6x *adc;
  AD5141 *dpot;
  bool factory;
  char buffer[BUFFERSIZE];
  char prevBuffer[BUFFERSIZE];
  uint8_t bufferIndex;
  void command();
  void cmd_set(char *args[]);
  void cmd_get(char *args[]);
  int checkValue(char* value);
};

#endif  //MCMILLANSERIAL_H