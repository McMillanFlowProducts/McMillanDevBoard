#define MCMILLAN_OTA
#define MCM_DEBUG
//#define MCM_DPOT

#include <Adafruit_NeoPixel.h>
#include "McMillanOTA.h"
#include "DACx0501.h"
#include "MCP3x6x.h"
#include "AD5144A.h"

#define MCM_SDA 41
#define MCM_SCL 40
#define MCM_SCK 36
#define MCM_MISO 37
#define MCM_MOSI 35
#define MCM_NEOPIXEL 48
#define ADC_CS 42

Adafruit_NeoPixel pixels(1, MCM_NEOPIXEL, NEO_GRB + NEO_KHZ800);
DACx0501 dac(DAC_ADDR_AGND, MCM_SDA, MCM_SCL);
MCP3x6x adc(MCM_SCK, MCM_MISO, MCM_MOSI, ADC_CS);
AD5141 dpot(0x77);
McMillanOTA ota;

const char * compiledOn = "compiled: " __DATE__ "\t" __TIME__;

long prevMillis = 0;
bool heartbeatLED = false;
bool demo = false;
bool demoDIR = true;
int demoValue = 0;

DACx0501Config dacconfig;

void setup(void) {
  Serial.begin(115200);
  Serial.println();
  Serial.println("McMillan Flow DEV");
  Serial.println(compiledOn);

  //pinMode(1, INPUT);  //enable analog read for valve

  dacconfig.REFDIV = true;
  dacconfig.GAIN = true;
  dacconfig.REF_PWDWN = false;
  dacconfig.DAC_PWDWN = false;

  Serial.print("DAC BEGIN: ");
  dac.begin(dacconfig);

  dac.setValue(0x0FAA);

#ifdef MCM_DPOT
  Serial.println("DPOT DEBUG:");
  Serial.print("Begin: ");
  Serial.println(dpot.begin());
  Serial.print("Connected: ");
  Serial.println(dpot.isConnected());
  Serial.print("CHANNELS:\t");
  Serial.println(dpot.pmCount());
  Serial.print("MAXVALUE:\t");
  Serial.println(dpot.maxValue());
  Serial.println();
#endif

  pixels.begin();
  pixels.setBrightness(20);

  ota.begin();

  delay(5000);
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
