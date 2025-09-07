#include "Pad.h"

namespace Pad {
NeoPixelBus<NeoGrbFeature, NeoEsp32BitBangWs2811Method> strip(PAD_SIZE, LED_PIN);
PCF8575 PCF(0x26);

bool dirty = false;
void onInterrupt() {
  dirty = true;
}

void init() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  Wire.setPins(I2C_SDA, I2C_SCL);
  Wire.begin();
  strip.Begin();
  strip.ClearTo(RgbColor(0, 0, 0));
  strip.Show();
  PCF.begin();

  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), onInterrupt, FALLING);
}

void clear() {
  strip.ClearTo(HslColor(0, 0, 0));
  strip.Show();
}

void clear(uint8_t r, uint8_t g, uint8_t b) {
  strip.ClearTo(RgbColor(r, g, b));
  strip.Show();
}

bool isDirty() {
  return dirty;
}

uint16_t getState() {
  uint16_t state = PCF.read16();
  dirty = false;
  return state;
}

void clearButtonLed(uint8_t idx) {
  strip.SetPixelColor(idx, HslColor(0, 0, 0));
  strip.Show();
}

void setButtonLed(uint8_t idx, color c, uint8_t blinkRate = BLINK_RATE::NONE) {
  strip.SetPixelColor(idx, HslColor(0, 0, 0));
  strip.Show();
}

void refresh() {
  static uint64_t lap = 0;

  if ((lap + (1000 / REFRESH_RATE)) > millis())
    return;

  lap = millis();
}

}  // namespace Pad

// void setup() {
//   Serial.begin(460800);
//   delay(2000);
//   Serial.println("hola");
//   delay(2000);
//   Serial.println("hola 2");
// }

// void loop() {
//   uint8_t i = 0;
//   uint16_t state = 0;
//   static uint16_t toggled = 0;
//   static uint16_t hue = 0;

//   // scan();
//   // for (float hue = 0; hue < 1; hue += inc) {
//   //   // Serial.printf("hue = %f \n", hue);
//   //   strip.SetPixelColor(i, colors[i]);
//   //   i = ++i % 16;
//   //   strip.Show();
//   //   delay(50);
//   // }

//   if (dirty) {

//     Serial.println(state, BIN);

//     for (uint8_t pin = 0; pin < 16; pin++) {
//       if (!bitRead(state, pin)) {
//         if (!bitRead(toggled, pin)) {
//           bitSet(toggled, pin);
//           strip.SetPixelColor(pin, HslColor(hue * 0.03125, 1, 0.005));
//           hue = ++hue % 32;
//         } else {
//           strip.SetPixelColor(pin, HslColor(0, 0, 0));
//           bitClear(toggled, pin);
//         }

//         Serial.print("toggled: ");
//         Serial.println(toggled, BIN);
//         strip.Show();
//       }
//     }

//     delay(50);
//     dirty = false;
//   }
// }