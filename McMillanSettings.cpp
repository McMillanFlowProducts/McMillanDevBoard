#include "McMillanSettings.h"

McMillanSettings::McMillanSettings() {}

bool McMillanSettings::begin() {
  if (!active) {
    prefs = new Preferences();
    if (!prefs->begin("McMPrefs", false)) {
      Serial.println("Preferences not loaded. RESTARTING...");
      ESP.restart();
    }
    active = true;
    if (!load()) {
      defaults();
      save();
    }
  }
  return true;
}

bool McMillanSettings::load() {
  if (active) {
    prefs->getString("model", settings.model, sizeof(settings.model));
    prefs->getString("serialNumber", settings.serialNumber, sizeof(settings.serialNumber));
    prefs->getDouble("sensorCali", settings.sensorCalibration);
    prefs->getDouble("setpointCali", settings.setpointCalibration);
    prefs->getDouble("dacCali", settings.DACCalibration);
    if (strlen(settings.serialNumber) > 0) {
      return true;
    }
  }
  return false;
}

bool McMillanSettings::save() {
  if (active) {
    prefs->putString("model", settings.model);
    prefs->putString("serialNumber", settings.serialNumber);
    prefs->putDouble("sensorCali", settings.sensorCalibration);
    prefs->putDouble("setpointCali", settings.setpointCalibration);
    prefs->putDouble("dacCali", settings.DACCalibration);
  }
  return false;
}

void McMillanSettings::defaults() {
  Serial.println("Loading Defaults...");
  strcpy(settings.model, "MCMILLAN DEV");
  strcpy(settings.serialNumber, "TEST12345678");
  settings.sensorCalibration = 1.0;
  settings.setpointCalibration = 1.0;
  settings.DACCalibration = 1.0;
}

char *McMillanSettings::getSerialNumber() {
  return settings.serialNumber;
}

void McMillanSettings::setSerialNumber(char *_serialNumber) {
  strcpy(settings.serialNumber, _serialNumber);
  save();
}

char *McMillanSettings::getModel() {
  return settings.model;
}

void McMillanSettings::setModel(char *_model) {
  strcpy(settings.model, _model);
  save();
}