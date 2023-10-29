//#define MCMILLAN_OTA
//#define MCM_DPOT

#include <Adafruit_NeoPixel.h>
#include "McMillanPins.h"
#include "McMillanOTA.h"
#include "McMillanSettings.h"
#include "McMillanSerial.h"
#include "DACx0501.h"
#include "MCP3x6x.h"
#include "AD5144A.h"

Adafruit_NeoPixel pixels(1, MCM_NEOPIXEL, NEO_GRB + NEO_KHZ800);
DACx0501 dac(DAC_ADDR_AGND, MCM_SDA, MCM_SCL);
MCP3x6x adc(MCM_SCK, MCM_MISO, MCM_MOSI, ADC_CS);
AD5141 dpot(0x77);
McMillanOTA ota;
McMillanSettings settings;
McMillanSerial mcmSerial(&settings, &dac, &adc, &dpot);

long prevMillis = 0;
bool heartbeatLED = false;

void setup(void) {
  pixels.begin();
  pixels.setBrightness(20);
  mcmSerial.begin();
  Serial.printf("Settings Loaded: %d\n", settings.begin());
  Serial.printf("SerialNumber: %s\n", settings.getSerialNumber());

  //pinMode(1, INPUT);  //enable analog read for valve

  Serial.print("DAC BEGIN: ");
  dac.begin({ true, true, false, false });

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

  ota.begin();

  //delay(5000);

  Serial.println("Leaving setup()...");
}

void heartbeat() {
  if (millis() - prevMillis >= 500) {
    // set color to red
    if (!heartbeatLED) {
      pixels.fill(0x00FF00);
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
  mcmSerial.loop();
}
