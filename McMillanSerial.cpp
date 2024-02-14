#include "McMillanSerial.h"

McMillanSerial::McMillanSerial(HWCDC* _USB, McMillanSettings* _settings, DACx0501* _dac, AD5141* _dpot, bool _addressMode)
  : USB(_USB), settings(_settings), dac(_dac), dpot(_dpot), addressMode(_addressMode), USBMode(true), Serial(nullptr) {
}

McMillanSerial::McMillanSerial(HardwareSerial* _Serial, McMillanSettings* _settings, DACx0501* _dac, AD5141* _dpot, bool _addressMode)
  : Serial(_Serial), settings(_settings), dac(_dac), dpot(_dpot), addressMode(_addressMode), USBMode(false), USB(nullptr) {
}

void McMillanSerial::begin() {
  factory = false;
  bufferIndex = 0;
  if (USBMode) {
    USB->begin(115200);
    while (!USB)
      ;
    USB->println();
  } else {
    Serial->begin(115200);
    while (!Serial)
      ;
    Serial->println();
  }
}

void McMillanSerial::begin(int baud) {
  factory = false;
  bufferIndex = 0;
  if (USBMode) {
    USB->begin(baud);
    while (!USB)
      ;
    USB->println();
  } else {
    Serial->begin(baud);
    while (!Serial)
      ;
    Serial->println();
  }
}

void McMillanSerial::begin(int baud, int rx, int tx) {
  factory = false;
  if (USBMode) {
    USB->begin(baud);
    while (!USB)
      ;
    USB->println();
  } else {
    Serial->begin(baud, SERIAL_8N1, rx, tx);
    Serial->println();
  }
  bufferIndex = 0;
}

void McMillanSerial::loop() {
  if (USBMode) {
    if (USB->available()) {
      if (bufferIndex >= BUFFERSIZE) {
        bufferIndex = 0;
      }
      char temp = USB->read();
      switch (temp) {
        case '\r':
          break;
        case '\n':
          buffer[bufferIndex] = '\0';
          bufferIndex = 0;
          command();
          break;
        default:
          buffer[bufferIndex++] = temp;
      }
    }
  } else {
    if (Serial->available()) {
      if (bufferIndex >= BUFFERSIZE) {
        bufferIndex = 0;
      }
      char temp = Serial->read();
      switch (temp) {
        case '\r':
          break;
        case '\n':
          buffer[bufferIndex] = '\0';
          bufferIndex = 0;
          command();
          break;
        default:
          buffer[bufferIndex++] = temp;
      }
    }
  }
}

void McMillanSerial::command() {
  strcpy(prevBuffer, buffer);
  char* cmd = strtok(buffer, " ");
  if (cmd != NULL) {
    char* args[MAXARGS];  //array of string ptrs
    int i = 0;
    args[i] = strtok(NULL, " ");
    while (args[i] != NULL && i < MAXARGS - 1) {
      //Serial->println(args[i]);
      i++;
      args[i] = strtok(NULL, " ");
    }
    if (strcasecmp(cmd, "set")) {
      cmd_set(args);
    } else if (strcasecmp(cmd, "get")) {
      cmd_get(args);
    } else if (strcasecmp(cmd, "save")) {
      settings->save();
      println("Saved Settings");
    } else if (strcasecmp(cmd, "load")) {
      settings->load();
      println("Loaded Settings");
    } else if (strcasecmp(cmd, "defaults") && factory) {
      settings->defaults();
      println("Loaded Defaults");
    } else {
      print("Unknown Command: ");
      println(prevBuffer);
    }
  } else {
    print("Unknown Command: ");
    println(prevBuffer);
  }
}

void McMillanSerial::cmd_set(char* args[]) {
  if (args[0] == NULL) {
    println("Not enough arguments");
  } else {
    uint8_t totalArgs = 0;
    for (int i = 0; i < MAXARGS && args[i] != NULL; i++) {
      totalArgs++;
    }
    switch (totalArgs) {
      case 1:
        {
          if (strcasecmp(args[0], "flag")) {
            println("Flag set");
          } else {
            printf("Unknown Command: %s\n", prevBuffer);
          }
        }
        break;
      case 2:
        {
          if (strcasecmp(args[0], "sn") && factory) {
            settings->setSerialNumber(args[1]);
            println(args[1]);
          } else if (strcasecmp(args[0], "model") && factory) {
            settings->setModel(args[1]);
            println(args[1]);
          } else if (strcasecmp(args[0], "factory")) {
            if (strcmp(args[1], "McM!ll@n") == 0) {
              factory = true;
              println("Factory Mode Enabled");
            }
          } else {
            printf("Unknown Command: %s\n", prevBuffer);
          }
        }
        break;
      default:
        if (strcasecmp(args[0], "dac") && factory) {
          int state = checkValue(args[2]);
          if (strcasecmp(args[1], "value")) {
            dac->setValue(atoi(args[2]));
            printf("Set DAC Value: %d\n", atoi(args[2]));
          } else if (state != -1) {
            if (strcasecmp(args[1], "refdiv")) {
              dac->setREFDIV(state);
              printf("Set DAC REFDIV: %d\n", state);
            } else if (strcasecmp(args[1], "gain")) {
              dac->setGain(state);
              printf("Set DAC GAIN: %d\n", state);
            } else if (strcasecmp(args[1], "refpwr")) {
              dac->setREF_PWDWN(state);
              printf("Set DAC PWR: %d\n", state);
            } else if (strcasecmp(args[1], "dacpwr")) {
              dac->setDAC_PWDWN(state);
              printf("Set DAC REF PWR: %d\n", state);
            } else {
              printf("Unknown Command: %s\n", prevBuffer);
            }
          } else {
            printf("Unknown Value: %s\n", args[2]);
          }
        } else if (strcasecmp(args[0], "adc") && factory) {
          println("Not yet implemented");
        } else if (strcasecmp(args[0], "dpot") && factory) {
          int state = checkValue(args[2]);
          if (strcasecmp(args[1], "value")) {
            //dpot->write(const uint8_t rdac, const uint8_t value)atoi(args[2]));
            printf("Set DPOT Value: %d\n", atoi(args[2]));
          } else if (state != -1) {
            printf("Unknown Command: %s\n", prevBuffer);
          } else {
            printf("Unknown Value: %s\n", args[2]);
          }
        } else {
          printf("Unknown Command: %s\n", prevBuffer);
        }
        break;
    }
  }
}

void McMillanSerial::cmd_get(char* args[]) {
  if (args[0] == NULL) {
    println("Not enough arguments");
  } else {
    uint8_t totalArgs = 0;
    for (int i = 0; i < MAXARGS && args[i] != NULL; i++) {
      totalArgs++;
    }
    switch (totalArgs) {
      case 1:
        {
          if (strcasecmp(args[0], "sn")) {
            printf("SerialNumber: %s\n", settings->getSerialNumber());
          } else if (strcasecmp(args[0], "model")) {
            printf("Model: %s\n", settings->getModel());
          } else if (strcasecmp(args[0], "dac") && factory) {
            printf("DAC Value: %d\n", dac->getValue());
          } else if (strcasecmp(args[0], "dpot") && factory) {
            uint8_t rdac;
            dpot->readBackRDAC(rdac);
            printf("DPOT Value: %d\n", rdac);
          } else {
            printf("Unknown Command: %s\n", prevBuffer);
          }
        }
        break;
      case 2:
        {
          if (strcasecmp(args[0], "dac") && factory) {
            if (strcasecmp(args[1], "refdiv")) {
              printf("DAC REFDIV: %d\n", dac->getREFDIV());
            } else if (strcasecmp(args[1], "gain")) {
              printf("DAC GAIN: %d\n", dac->getGain());
            } else if (strcasecmp(args[1], "refpwr")) {
              printf("DAC REF PWDWN: %d\n", dac->getREF_PWDWN());
            } else if (strcasecmp(args[1], "dacpwr")) {
              printf("DAC PWRDWN: %d\n", dac->getDAC_PWDWN());
            } else if (strcasecmp(args[1], "value")) {
              printf("DAC Value: %d\n", dac->getValue());
            } else {
              printf("Unknown Command: %s\n", prevBuffer);
            }
          } else if (strcasecmp(args[0], "adc") && factory) {
            println("Not yet implemented");
          } else if (strcasecmp(args[0], "dpot") && factory) {
            println("Not yet implemented");
          }else {
            printf("Unknown Command: %s\n", prevBuffer);
          }
        }
        break;
      default:
        break;
    }
  }
}

int McMillanSerial::checkValue(char* value) {
  if (strcasecmp(value, "on") || strcasecmp(value, "1")) {
    return 1;
  } else if (strcasecmp(value, "off") || strcasecmp(value, "0")) {
    return 0;
  } else {
    return -1;
  }
}

bool McMillanSerial::strcasecmp(char* string1, const char* string2) {
  for (uint8_t cnt = 0; cnt < strlen(string1); cnt++) {
    string1[cnt] = tolower(string1[cnt]);
  }
  return strcmp(string1, string2) == 0;
}

template<typename T>
void McMillanSerial::print(T _print) {
  if (USBMode) {
    USB->print(_print);
  } else {
    Serial->print(_print);
  }
}

template<typename T>
void McMillanSerial::printf(const char* format, T _print) {
  if (USBMode) {
    USB->printf(format, _print);
  } else {
    Serial->printf(format, _print);
  }
}

template<typename T>
void McMillanSerial::println(T _print) {
  if (USBMode) {
    USB->println(_print);
  } else {
    Serial->println(_print);
  }
}

McMillanSerial::~McMillanSerial() {}
