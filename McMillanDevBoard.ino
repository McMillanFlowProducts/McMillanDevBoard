//#define MCMILLAN_OTA

#include <Adafruit_NeoPixel.h>
#include "McMillanOTA.cpp"
#include "DACx0501.h"
#include <Wire.h>

Adafruit_NeoPixel pixels(1, 48, NEO_GRB + NEO_KHZ800);
DACx0501 dac(DAC_16, 0x48, 41, 40);
McMillanOTA ota;

long prevMillis = 0;
bool heartbeatLED = false;
bool demo = false;
bool demoDIR = true;
int demoValue = 0;

void setup(void) {
  Serial.begin(115200);
  Serial.println("hello world");
  pinMode(1, INPUT);
  dac.begin();

  pixels.begin();
  pixels.setBrightness(20);
  ota.begin();
}

void heartbeat() {
  if (millis() - prevMillis >= 500) {
    // set color to red
    if (!heartbeatLED) {
      if (!demo) {
        pixels.fill(0x00FF00);
      } else {
        pixels.fill(0xFF0000);
      }
      pixels.show();
    } else {
      pixels.fill(0x000000);
      pixels.show();
    }
    heartbeatLED = !heartbeatLED;
    prevMillis = millis();
  }
}

void loop(void) {
  ota.loop();
  heartbeat();

  Serial.println(analogRead(1));

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
}
