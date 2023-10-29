#ifndef MCMILLANSETTINGS_H
#define MCMILLANSETTINGS_H

#include <Preferences.h>

struct McMSettings {
  char model[32];
  char serialNumber[32];
  double sensorCalibration;
  double setpointCalibration;
  double DACCalibration;
};

class McMillanSettings {
public:
  McMillanSettings();
  bool begin();
  bool load();
  bool save();
  void setSerialNumber(char *_serialNumber);
  char *getSerialNumber();
  void setModel(char *_model);
  char *getModel();
  void defaults();
protected:
  bool active;
  Preferences *prefs;
  McMSettings settings;
};

#endif  //MCMILLANSETTINGS_H