#define MCMILLAN_OTA

#include <Adafruit_NeoPixel.h>
#include "McMillanOTA.cpp"

Adafruit_NeoPixel pixels(1, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
McMillanOTA ota;
long prevMillis = 0;
bool heartbeatLED = false;

void setup(void) {
  Serial.begin(115200);

#if defined(NEOPIXEL_POWER)
  // If this board has a power control pin, we must set it to output and high
  // in order to enable the NeoPixels. We put this in an #if defined so it can
  // be reused for other boards without compilation errors
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, HIGH);
#endif

  pixels.begin();
  pixels.setBrightness(20);
  ota.begin();
}

void heartbeat() {
  if (millis() - prevMillis >= 500) {
    // set color to red
    if (!heartbeatLED) {
      pixels.fill(0xFF0000);
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
