#ifndef __PAD__
#define __PAD__

#include <NeoPixelBus.h>

#include "Arduino.h"
#include "PCF8575.h"
#include "Wire.h"

#define I2C_SDA 5
#define I2C_SCL 4
#define LED_PIN 2
#define PAD_SIZE 16
#define INTERRUPT_PIN 7
#define _PCF_ADDR 0x26
#define REFRESH_RATE 12  // 12 fps

namespace Pad {
enum BLINK_RATE {
  NONE = 0,
  VERY_FAST = 4,
  FAST = 8,
  NORMAL = 12,
  SLOW = 16,
  VERY_SLOW = 24
};

struct color {
  float h;
  float s;
  float l;
};

void init();
uint16_t getState();
bool isDirty();
void clear();
void clear(uint8_t r, uint8_t g, uint8_t b);
void update();

void clearButtonLed(uint8_t idx);
void setButtonLed(uint8_t idx, color c, uint8_t bliknRate);
}  // namespace Pad

#endif
