#define MCMILLAN_OTA

#include <Adafruit_NeoPixel.h>
#include "McMillanOTA.cpp"
#include "DACx0501SPI.h"

Adafruit_NeoPixel pixels(1, 48, NEO_GRB + NEO_KHZ800);
DACx0501SPI dac(36, 35, 42);
McMillanOTA ota;

long prevMillis = 0;
bool heartbeatLED = false;
bool demo = false;

void setup(void) {
  dac.begin();
  Serial.begin(115200);
  Serial.println("hello world");

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
          int gain;
          switch (Serial.parseInt()) {
            case '0':
              gain = 0x0000;
              break;
            case 1:
              gain = 0x0001;
              break;
            case 10:
              gain = 0x0100;
              break;
            case 11:
              gain = 0x0101;
              break;
            default:
              gain = 0x0000;
          }
          Serial.printf("GAIN: 0x%04X\n", gain);
          //dac.spiCommand(dac.dacspi, CMD_GAIN, gain);
        }
        break;
      case 'r':
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
      case 'd':
        demo = !demo;
    }
  }
  if (demo) {
    for (int i = 0; i <= 0xFFFF; i += 100) {
      dac.set(i);
    }
    for (int i = 0xFFFF; i >= 0; i -= 100) {
      dac.set(i);
    }
  }
}




