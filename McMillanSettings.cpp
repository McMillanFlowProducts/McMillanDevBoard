#include "McMillanSettings.h"

McMillanSettings::McMillanSettings() {}

bool McMillanSettings::begin() {
  if (!active) {
    prefs = new Preferences();
    if (!prefs->begin("McMPrefs", false)) {
      //Serial.println("Preferences not loaded. RESTARTING...");
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
    prefs->getChar("address", settings.address);
    prefs->getDouble("sensorCali", settings.sensorCalibration);
    prefs->getDouble("setpointCali", settings.setpointCalibration);
    prefs->getDouble("dacCali", settings.DACCalibration);
    prefs->getDouble("digital", settings.digitalSetpoint);
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
    prefs->putChar("address", settings.address);
    prefs->putDouble("sensorCali", settings.sensorCalibration);
    prefs->putDouble("setpointCali", settings.setpointCalibration);
    prefs->putDouble("dacCali", settings.DACCalibration);
    prefs->putDouble("digital", settings.digitalSetpoint);
  }
  return false;
}

void McMillanSettings::defaults() {
  Serial.println("Loading Defaults...");
  strcpy(settings.model, "McMillan DEV Board");
  strcpy(settings.serialNumber, "TEST12345678");
  settings.address = 1;
  settings.sensorCalibration = 1.0;
  settings.setpointCalibration = 1.0;
  settings.DACCalibration = 1.0;
  settings.digitalSetpoint = 1.0;
}

char *McMillanSettings::getSerialNumber() {
  return settings.serialNumber;
}

void McMillanSettings::setSerialNumber(char *_serialNumber) {
  strcpy(settings.serialNumber, _serialNumber);
}

char *McMillanSettings::getModel() {
  return settings.model;
}

void McMillanSettings::setModel(char *_model) {
  strcpy(settings.model, _model);
}

char McMillanSettings::getAddress() {
  return settings.address;
}

void McMillanSettings::setAddress(char _address) {
  settings.address = _address;
}

double McMillanSettings::getSensorCalibration() {
  return settings.sensorCalibration;
}

double McMillanSettings::getSetpointCalibration() {
  return settings.setpointCalibration;
}

double McMillanSettings::getDACCalibration() {
  return settings.DACCalibration;
}

double McMillanSettings::getDigitalSetpoint() {
  return settings.digitalSetpoint;
}

void McMillanSettings::setSensorCalibration(double value) {
  settings.sensorCalibration = value;
}

void McMillanSettings::setSetpointCalibration(double value) {
  settings.setpointCalibration = value;
}

void McMillanSettings::setDACCalibration(double value) {
  settings.DACCalibration = value;
}

void McMillanSettings::setDigitalSetpoint(double value) {
  settings.digitalSetpoint = value;
}

McMillanSettings::~McMillanSettings() {
  delete prefs;
}