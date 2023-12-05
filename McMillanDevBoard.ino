#define MCM_DEBUG
//#define MCM_DPOT

#include "McMillanConfig.h"
#include "McMillanOTA.h"
#include "McMillanSettings.h"
#include "McMillanSerial.h"
#include "DACx0501.h"
#include "MCP3x6x.h"
#include "AD5144A.h"
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels(1, MCM_NEOPIXEL, NEO_GRB + NEO_KHZ800);
SPIClass spi = SPIClass(HSPI);
DACx0501 dac(DAC_ADDR_AGND, MCM_SDA, MCM_SCL);
MCP3462 adc(ADC_CS, &spi, MCM_MOSI, MCM_MISO, MCM_SCK);
AD5141 dpot(0x20);
McMillanSettings settings;
McMillanSerial mcmUSB(&Serial, &settings, &dac, &adc, &dpot);
McMillanSerial mcmRS485(&Serial0, &settings, &dac, &adc, &dpot, true);
McMillanOTA ota;
//McMillanSerial *coms[2] = {&mcmUSB, &mcmRS485};

long prevMillis = 0;
bool heartbeatLED = false;

void setup(void) {
  pixels.begin();
  pixels.setBrightness(20);
  mcmUSB.begin();
  mcmRS485.begin();

#ifdef MCM_DEBUG
  Serial.printf("Settings Loaded: %d\n", settings.begin());
  Serial.printf("Model: %s\n", settings.getModel());
  Serial.printf("SerialNumber: %s\n", settings.getSerialNumber());
  Serial.println("compiled: " __DATE__ " " __TIME__);

  ota.begin();

  //pinMode(1, INPUT);  //enable analog read for valve

 /*  Wire.begin(MCM_SDA, MCM_SCL, 10000000);
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning for I2C devices ...");
  for (address = 0x01; address < 0x7f; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.printf("I2C device found at address 0x%02X\n", address);
      nDevices++;
    } else if (error != 2) {
      Serial.printf("Error %d at address 0x%02X\n", error, address);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found");
  } */

  Serial.printf("DAC BEGIN: %d\n", dac.begin());
  dac.setValue(0xFFAA);


  try {
    Serial.println((adc.begin()) ? "ADC Started" : "ADC Failed");
  } catch (...) {
    Serial.println("ADC Failed");
  }


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
  ota.begin();
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
  ota.loop();
  heartbeat();
  mcmUSB.loop();
  mcmRS485.loop();

  /*int32_t adcdata = adc.analogRead(MCP_CH0);
  // Convert the analog reading
  double voltage = adcdata * adc.getReference() / adc.getMaxValue();
  // print out the value you read:
  Serial.print("voltage: ");
  Serial.println(voltage, 10);

  delay(100);
  */
}
