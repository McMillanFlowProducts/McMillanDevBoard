//#define MCMILLAN_OTA

#include <Adafruit_NeoPixel.h>
#include "McMillanOTA.cpp"
#include "DACx0501SPI.h"
#include <Wire.h>

Adafruit_NeoPixel pixels(1, 48, NEO_GRB + NEO_KHZ800);
//DACx0501SPI dac(DAC_16, 36, 35, 42);
McMillanOTA ota;

long prevMillis = 0;
bool heartbeatLED = false;
bool demo = false;

void setup(void) {
  Serial.begin(115200);
  Serial.println("hello world");

  Wire.begin(35, 36, 1000000);

  
  Wire.beginTransmission(0x48);
  Wire.write(DAC_CMD_CONFIG);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
  
  Wire.beginTransmission(0x48);
  Wire.write(DAC_CMD_GAIN);
  Wire.write(0x00);
  Wire.write(0x01);
  Wire.endTransmission();

  Wire.beginTransmission(0x48);
  Wire.write(DAC_CMD_DACn);
  Wire.write(0xFF);
  Wire.write(0xFF);
  Wire.endTransmission();

  Wire.beginTransmission(0x48);
  Wire.write(DAC_CMD_DEVID);
  Wire.endTransmission();

  Wire.requestFrom(0x48, 2);
  Serial.println(Wire.read(), BIN);
  Serial.println(Wire.read(), BIN);

  Wire.beginTransmission(0x48);
  Wire.write(DAC_CMD_STATUS);
  Wire.endTransmission();

  Wire.requestFrom(0x48, 2);
  Serial.println(Wire.read(), BIN);
  Serial.println(Wire.read(), BIN);
  //dac.begin();

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

  /*
  while (Serial.available()) {
    switch (Serial.read()) {
      case 'i':
        {
          int value = Serial.parseInt();
          Serial.printf("DAC: 0x%04X, %04d\n", value, value);
          dac.set(value);
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
              dac.disableREF(0);
              Serial.println("IREF ON");
              break;
           default:
              dac.disableREF(1);
              Serial.println("IREF OFF");
          }
        }
        break;
      case 'q':
        {
          switch (Serial.parseInt()) {
            case 0:
              dac.disableDAC(0);
              Serial.println("DAC ON");
              break;
            default:
              dac.disableDAC(1);
              Serial.println("DAC OFF");
          }
        }
        break;
      case 'd':
        demo = !demo;
    }
  }*/
  if (demo) {
    for (int i = 0; i <= 0xFFFF; i += 100) {
      Wire.beginTransmission(0x48);
      Wire.write(DAC_CMD_DACn);
      Wire.write(i >> 8);
      Wire.write(i & 0xFF);
      Wire.endTransmission();
    }
    for (int i = 0xFFFF; i >= 0; i -= 100) {
      Wire.beginTransmission(0x48);
      Wire.write(DAC_CMD_DACn);
      Wire.write(i >> 8);
      Wire.write(i & 0xFF);
      Wire.endTransmission();
    }
  }
}
