//#define MCMILLAN_OTA
#define PIN_NEOPIXEL 48
#define DAC_CS 42

#include <Adafruit_NeoPixel.h>
#include "McMillanOTA.cpp"
#include <SPI.h>

Adafruit_NeoPixel pixels(1, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
McMillanOTA ota;
SPIClass *dacspi = NULL;
static const int spiClk = 10000000;  // 10 MHz
//DAC Command Bytes
//static const byte NOOP    =  0x00;
//static const byte DEVID   =  0x01;
static const byte SYNC = 0x02;
static const byte CONFIG = 0x03;
static const byte GAIN = 0x04;
static const byte TRIGGER = 0x05;
//static const byte STATUS  =  0x06;
static const byte DACn = 0x08;

long prevMillis = 0;
bool heartbeatLED = false;
bool demo = false;

void setup(void) {
  dacspi = new SPIClass(HSPI);
  dacspi->begin(36, 37, 35, 42);  //SCLK, MISO, MOSI, SS
  pinMode(dacspi->pinSS(), OUTPUT);
  digitalWrite(dacspi->pinSS(), HIGH);

  Serial.begin(115200);
  Serial.println("hello world");
  Serial.println(0xFABC >> 8, HEX);
  Serial.println(0xFABC & 0xFF, HEX);

  ;
  spiCommand(dacspi, SYNC, 0x0000);
  spiCommand(dacspi, CONFIG, 0x0000);
  spiCommand(dacspi, GAIN, 0x0101);
  spiCommand(dacspi, TRIGGER, 0x0000);
  spiCommand(dacspi, DACn, 0xAAFF);

  //delay(10000);

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
      //spiCommand(dacspi, CONFIG, 0x0000);
    } else {
      pixels.fill(0x000000);
      pixels.show();
      //spiCommand(dacspi, CONFIG, 0x0100);
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
          spiCommand(dacspi, DACn, value);
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
          spiCommand(dacspi, GAIN, gain);
        }
        break;
      case 'r':
        {
          switch (Serial.parseInt()) {
            case 0:
              spiCommand(dacspi, CONFIG, 0x0000);
              Serial.println("IREF ON");
              break;
            default:
              spiCommand(dacspi, CONFIG, 0x0100);
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
      spiCommand(dacspi, DACn, i);
    }
    for (int i = 0xFFFF; i >= 0; i -= 100) {
      spiCommand(dacspi, DACn, i);
    }
  }
}



void spiCommand(SPIClass *spi, byte cmd, int data) {
  //use it as you would the regular arduino SPI API
  spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE3));
  digitalWrite(spi->pinSS(), LOW);  //pull SS slow to prep other end for transfer
  spi->transfer(cmd);
  spi->transfer(data >> 8);
  spi->transfer(data & 0xFF);
  digitalWrite(spi->pinSS(), HIGH);  //pull ss high to signify end of data transfer
  spi->endTransaction();
}
