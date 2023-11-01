#include "McMillanSerial.h"

McMillanSerial::McMillanSerial(HWCDC* _USB, McMillanSettings* _settings, DACx0501* _dac, MCP3x6x* _adc, AD5141* _dpot, bool _multiple)
  : USB(_USB), settings(_settings), dac(_dac), adc(_adc), dpot(_dpot), multiple(_multiple) {
  USBMode = true;
}

McMillanSerial::McMillanSerial(HardwareSerial* _Serial, McMillanSettings* _settings, DACx0501* _dac, MCP3x6x* _adc, AD5141* _dpot, bool _multiple)
  : Serial(_Serial), settings(_settings), dac(_dac), adc(_adc), dpot(_dpot), multiple(_multiple) {
  USBMode = false;
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
            print("Unknown Command: ");
            println(prevBuffer);
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
            print(args[1]);
          } else if (strcasecmp(args[0], "factory")) {
            if (strcmp(args[1], "McM!ll@n") == 0) {
              factory = true;
              println("Factory Mode Enabled");
            }
          } else {
            print("Unknown Command: ");
            println(prevBuffer);
          }
        }
        break;
      default:
        if (strcasecmp(args[0], "dac") && factory) {
          int state = checkValue(args[2]);
          if (strcasecmp(args[1], "value")) {
            dac->setValue(atoi(args[2]));
            print("Set DAC Value: ");
            println(atoi(args[2]));
          } else if (state != -1) {
            if (strcasecmp(args[1], "refdiv")) {
              dac->setREFDIV(state);
              print("Set DAC REFDIV: ");
              println(state);
            } else if (strcasecmp(args[1], "gain")) {
              dac->setGain(state);
              print("Set DAC GAIN: ");
              println(state);
            } else if (strcasecmp(args[1], "refpwr")) {
              dac->setREF_PWDWN(state);
              print("Set DAC PWR: ");
              println(state);
            } else if (strcasecmp(args[1], "dacpwr")) {
              dac->setDAC_PWDWN(state);
              print("Set DAC REF PWR: ");
              println(state);
            } else {
              print("Unknown Command: ");
              println(prevBuffer);
            }
          } else {
            print("Unknown Value: %s\n");
            println(args[2]);
          }
        } else {
          print("Unknown Command: ");
          println(prevBuffer);
        }
        break;
    }
  }
}

void McMillanSerial::cmd_get(char* args[]) {
  if (args[0] == NULL) {
    println("ERR: Not enough arguments");
  } else {
    uint8_t totalArgs = 0;
    for (int i = 0; i < MAXARGS && args[i] != NULL; i++) {
      totalArgs++;
    }
    switch (totalArgs) {
      case 1:
        {
          if (strcasecmp(args[0], "sn")) {
            Serial->printf("SerialNumber: %s\n", settings->getSerialNumber());
          } else if (strcasecmp(args[0], "model")) {
            Serial->printf("Model: %s\n", settings->getModel());
          } else if (strcasecmp(args[0], "dac")) {
            Serial->printf("DAC Value: %d\n", dac->getValue());
          } else {
            print("Unknown Command: ");
            println(prevBuffer);
          }
        }
        break;
      case 2:
        {
          if (strcasecmp(args[0], "dac")) {
            if (strcasecmp(args[1], "refdiv")) {
              Serial->printf("DAC REFDIV: %d\n", dac->getREFDIV());
            } else if (strcasecmp(args[1], "gain")) {
              Serial->printf("DAC GAIN: %d\n", dac->getGain());
            } else if (strcasecmp(args[1], "refpwr")) {
              Serial->printf("DAC REF PWDWN: %d\n", dac->getREF_PWDWN());
            } else if (strcasecmp(args[1], "dacpwr")) {
              Serial->printf("DAC PWRDWN: %d\n", dac->getDAC_PWDWN());
            } else if (strcasecmp(args[1], "value")) {
              Serial->printf("DAC Value: %d\n", dac->getValue());
            } else {
              print("Unknown Command: ");
              println(prevBuffer);
            }
          } else {
            print("Unknown Command: ");
            println(prevBuffer);
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

bool McMillanSerial::strcasecmp(char* string1, char* string2) {
  for (uint8_t cnt = 0; cnt < strlen(string1); cnt++) {
    string1[cnt] = tolower(string1[cnt]);
  }
  return strcmp(string1, string2) == 0;
}

size_t McMillanSerial::print(char* _print) {
  if (USBMode) {
    return USB->print(_print);
  } else {
    return Serial->print(_print);
  }
}

void McMillanSerial::println(char* _print) {
  if (USBMode) {
    USB->println(_print);
  } else {
    Serial->println(_print);
  }
}

void McMillanSerial::println(int _print) {
  if (USBMode) {
    USB->println(_print);
  } else {
    Serial->println(_print);
  }
}
