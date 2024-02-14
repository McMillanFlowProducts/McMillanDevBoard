#define MCM_DEBUG
#define MCM_ADC
//#define MCM_DPOT

#include "McMillanConfig.h"
#include "McMillanSettings.h"
#include "McMillanSerial.h"
#include "DACx0501.h"
#include "AD5144A.h"
#include <Adafruit_NeoPixel.h>

#ifdef MCM_ADC
#include "MCP3x6x.hpp"
SPIClass spi = SPIClass(HSPI);
MCP3462 adc(ADC_CS, &spi, MCM_MOSI, MCM_MISO, MCM_SCK);
#endif //MCM_ADC

Adafruit_NeoPixel pixels(1, MCM_NEOPIXEL, NEO_GRB + NEO_KHZ800);
DACx0501 dac(DAC_ADDR_AGND, MCM_SDA, MCM_SCL);
AD5141 dpot(0x20);
McMillanSettings settings;
McMillanSerial mcmUSB(&Serial, &settings, &dac, &dpot);
McMillanSerial mcmRS485(&Serial0, &settings, &dac, &dpot, true);

long prevMillis = 0;
bool heartbeatLED = false;
long counter = 0;

void setup(void) {
  pixels.begin();
  pixels.setBrightness(20);
  mcmUSB.begin();
  //mcmRS485.begin();

#ifdef MCM_DEBUG
  Serial.printf("Settings Loaded: %d\n", settings.begin());
  Serial.printf("Model: %s\n", settings.getModel());
  Serial.printf("SerialNumber: %s\n", settings.getSerialNumber());
  Serial.println("compiled: " __DATE__ " " __TIME__);

  //ota.begin();

  //pinMode(1, INPUT);  //enable analog read for valve

  Serial.printf("DAC BEGIN: %d\n", dac.begin());
  dac.setValue(14896);  //5.000v == 14896  4.096v == 12203  2.5v == 7449

#ifdef MCM_ADC
  try {
    Serial.println((adc.begin(0)) ? "ADC Started" : "ADC Failed");
  } catch (...) {
    Serial.println("ADC Failed");
  }
#endif  //MCM_ADC

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
#endif  //MCM_DPOT
  delay(2000);
  Serial.println("Leaving setup()...");

#else  //MCM_DEBUG
  settings.begin();
  dac.begin();
  adc.begin();
  //ota.begin();
#ifdef MCM_DPOT
  dpot.begin();
#endif  //MCM_DPOT
#endif  //MCM_DEBUG
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
  //ota.loop();
  heartbeat();
  mcmUSB.loop();
  //mcmRS485.loop();

#ifdef MCM_ADC
  int32_t adcdata = adc.analogRead(MCP_TEMP);
  // Convert the analog reading
  double voltage = adcdata * adc.getReference() / adc.getMaxValue();
  // print out the value you read:
  Serial.printf("%06d > %1.3fv >> %09d\n", adcdata, voltage, ++counter);

  delay(1000);
#endif  //MCM_ADC
}
