#define MCMILLAN_OTA
#define PIN_NEOPIXEL  48

#include <Adafruit_NeoPixel.h>
#include "McMillanOTA.cpp"

Adafruit_NeoPixel pixels(1, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
McMillanOTA ota;
long prevMillis = 0;
bool heartbeatLED = false;

void setup(void) {
  Serial.begin(115200);
  //Serial1.begin(115200);
  Serial.println("hello world");

  pixels.begin();
  pixels.setBrightness(20);
  ota.begin();
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
}
