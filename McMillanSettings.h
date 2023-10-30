#ifndef MCMILLANSETTINGS_H
#define MCMILLANSETTINGS_H

#include <Preferences.h>

struct McMSettings {
  char model[32];
  char serialNumber[32];
  char address;
  double sensorCalibration;
  double setpointCalibration;
  double DACCalibration;
  double digitalSetpoint;
};

class McMillanSettings {
public:
  McMillanSettings();
  bool begin();
  bool load();
  bool save();
  void defaults();
  void setSerialNumber(char *_serialNumber);
  char *getSerialNumber();
  void setModel(char *_model);
  char *getModel();
  char getAddress();
  void setAddress(char _address);
  double getSensorCalibration();
  double getSetpointCalibration();
  double getDACCalibration();
  double getDigitalSetpoint();
  void setSensorCalibration(double value);
  void setSetpointCalibration(double value);
  void setDACCalibration(double value);
  void setDigitalSetpoint(double value);
protected:
  bool active;
  Preferences *prefs;
  McMSettings settings;
};

#endif  //MCMILLANSETTINGS_H