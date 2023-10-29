#include "McMillanSerial.h"

McMillanSerial::McMillanSerial(McMillanSettings* _settings, DACx0501* _dac, MCP3x6x* _adc)
  : settings(_settings), dac(_dac), adc(_adc) {
}

void McMillanSerial::begin() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("McMillan Flow DEV");
  Serial.println("compiled: " __DATE__ "\t" __TIME__);
  bufferIndex = 0;
  demo = false;
  demoDIR = true;
  demoValue = 0;
}

void McMillanSerial::loop() {
  if (Serial.available()) {
    if (bufferIndex >= BUFFERSIZE) {
      bufferIndex = 0;
    }
    char temp = Serial.read();
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
  /*
while (Serial.available()) {
    switch (Serial.read()) {
      case 'i':
        {
          int value = Serial.parseInt();
          Serial.printf("DAC: 0x%04X, %04d\n", value, value);
          dac.setValue(value);
        }
        break;
      case 'g':
        {
          switch (Serial.parseInt()) {
            case 0:
              dac.setGain(0);
              Serial.println("GAIN 1");
              break;
            default:
              dac.setGain(1);
              Serial.println("GAIN 2");
          }
        }
        break;
      case 'r':
        {
          switch (Serial.parseInt()) {
            case 0:
              dac.setREFDIV(0);
              Serial.println("REFDIV 1");
              break;
            default:
              dac.setREFDIV(1);
              Serial.println("REFDIV 2");
          }
        }
        break;
      case 'p':
        {
          switch (Serial.parseInt()) {
            case 0:
              dac.setREF_PWDWN(0);
              Serial.println("IREF ON");
              break;
            default:
              dac.setREF_PWDWN(1);
              Serial.println("IREF OFF");
          }
        }
        break;
      case 'q':
        {
          switch (Serial.parseInt()) {
            case 0:
              dac.setDAC_PWDWN(0);
              Serial.println("DAC ON");
              break;
            default:
              dac.setDAC_PWDWN(1);
              Serial.println("DAC OFF");
          }
        }
        break;
      case 'd':
        demo = !demo;
    }
  }
  if (demo) {
    dac.setValue(demoValue);
    if (demoDIR) {
      demoValue += 10;
    } else {
      demoValue -= 10;
    }
    if (demoValue <= 0 || demoValue >= 0xFFFF) {
      demoDIR = !demoDIR;
      if (demoValue < 1) {
        demoValue = 0;
      } else {
        demoValue = 0xFFFF;
      }
    }
  }
*/
}

void McMillanSerial::command() {
  //Serial.println(buffer);
  char* cmd = strtok(buffer, delim);
  Serial.println(cmd);
  if (cmd != NULL) {
    char* args[MAXARGS];  //array of string ptrs
    int i = 0;
    args[i] = strtok(NULL, delim);
    while (args[i] != NULL && i < MAXARGS - 1) {
      //Serial.println(args[i]);
      i++;
      args[i] = strtok(NULL, delim);
    }
    if (strcasecmp(cmd, "set")) {
      cmd_set(args);
    } else if (strcasecmp(cmd, "get")) {
      cmd_get(args);
    } else if (strcasecmp(cmd, "save")) {
      settings->save();
    } else if (strcasecmp(cmd, "load")) {
      settings->load();
    } else if (strcasecmp(cmd, "defaults")) {
      settings->defaults();
    }
  } else {
    Serial.println("NO CMD FOUND");
  }
}

void McMillanSerial::cmd_set(char* args[]) {
  if (args[0] == NULL) {
    Serial.println("ERR: Not enough arguments");
  } else {
    uint8_t totalArgs = 0;
    for (int i = 0; i < MAXARGS && args[i] != NULL; i++) {
      totalArgs++;
    }
    switch (totalArgs) {
      case 1:
        {
          if (strcasecmp(args[0], "flag")) {
            Serial.println("Flag set");
          }
        }
        break;
      case 2:
        {
          if (strcasecmp(args[0], "sn")) {
            settings->setSerialNumber(args[1]);
            Serial.print("Set SerialNumber: ");
          } else if (strcasecmp(args[0], "model")){
            settings->setModel(args[1]);
            Serial.print("Set Model: ");
          }
          Serial.println(args[1]);
        }
        break;
      default:
        break;
    }
  }
}

void McMillanSerial::cmd_get(char* args[]) {
  if (args[0] == NULL) {
    Serial.println("ERR: Not enough arguments");
  } else {
    uint8_t totalArgs = 0;
    for (int i = 0; i < MAXARGS && args[i] != NULL; i++) {
      totalArgs++;
    }
    switch (totalArgs) {
      case 1:
        {
          if (strcasecmp(args[0], "sn")) {
            Serial.printf("SerialNumber: %s\n", settings->getSerialNumber());
          } else if (strcasecmp(args[0], "model")) {
            Serial.printf("Model: %s\n", settings->getModel());
          }
        }
        break;
      default:
        break;
    }
  }
}

bool McMillanSerial::strcasecmp(char* string1, char* string2) {
  for (uint8_t cnt = 0; cnt < strlen(string1); cnt++) {
    string1[cnt] = tolower(string1[cnt]);
  }
  return strcmp(string1, string2) == 0;
}
