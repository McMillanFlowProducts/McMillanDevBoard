#ifndef MCMILLANSERIAL_H
#define MCMILLANSERIAL_H

#include "DACx0501.h"
#include "MCP3x6x.h"
#include "McMillanSettings.h"

//const char *compiledOn = "compiled: " __DATE__ "\t" __TIME__;
const int BUFFERSIZE = 64;
const char delim[] = " ";
const char MAXARGS = 4;

class McMillanSerial {
public:
  McMillanSerial(McMillanSettings *_settings, DACx0501 *_dac, MCP3x6x *_adc);
  void begin();
  void loop();
  bool strcasecmp(char *string1, char *string2);
protected:
  McMillanSettings *settings;
  DACx0501 *dac;
  MCP3x6x *adc;
  bool demo, demoDIR;
  int demoValue;
  char buffer[BUFFERSIZE];
  uint8_t bufferIndex;
  void command();
  void cmd_set(char *args[]);
  void cmd_get(char *args[]);
};

#endif  //MCMILLANSERIAL_H