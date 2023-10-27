#ifndef MCMILLANSETTINGS_H
#define MCMILLANSETTINGS_H

#include "EEPROM.h"

class McMillanSettings {
public:
  McMillanSettings();
  bool load();
  bool save();
protected:
  EEPROMClass *settings;
};

#endif  //MCMILLANSETTINGS_H