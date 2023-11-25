#include <Arduino.h>

#include <RTClib.h>

#include <FastLED.h> // http://fastled.io/docs/3.1/functions.html

#include "util.h"

CRGB leds[NUM_LEDS];

static RTC_DS1307 rtc;
static DateTime now;
static DateTime then;
static char print_buf[30];

static void error()
{
    for (;;)
    {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        delay(125);
    }
}

void light_block(const CRGB& colour, const uint8_t n, const int8_t x_sw, const int8_t y_sw)
{
  switch (n)
  {
    case 9: leds[xy(x_sw + 0, y_sw + 2)] = colour;
    case 8: leds[xy(x_sw + 0, y_sw + 1)] = colour;
    case 7: leds[xy(x_sw + 0, y_sw + 0)] = colour;
    case 6: leds[xy(x_sw + 1, y_sw + 2)] = colour;
    case 5: leds[xy(x_sw + 1, y_sw + 1)] = colour;
    case 4: leds[xy(x_sw + 1, y_sw + 0)] = colour;
    case 3: leds[xy(x_sw + 2, y_sw + 2)] = colour;
    case 2: leds[xy(x_sw + 2, y_sw + 1)] = colour;
    case 1: leds[xy(x_sw + 2, y_sw + 0)] = colour;
  }
}

auto thousands = [](uint32_t n){ return (n % 10000UL) / 1000UL; };
auto hundreds = [](uint32_t n){ return (n % 1000UL) / 100UL; };
auto tens = [](uint32_t n){ return (n % 100UL) / 10UL; };
auto units = [](uint32_t n){ return n % 10ULL; };

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  if (!rtc.begin()) error();

  FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  now = rtc.now();
  then = now - TimeSpan(60); // make sure we have at least 1 minute difference
}

void loop()
{
  // wait until the next second
  while (true)
  {
    now = rtc.now();

    if (now.second() != then.second()) break;
    else delay(500);
  }

  // only write new leds if the minute has changed
  if (now.minute() != then.minute())
  {
    for_range(i, NUM_LEDS) leds[i] = CRGB::Black;

    light_block(CRGB::Red, thousands(now.year()), -2, 0);
    light_block(CRGB::Red, tens(now.day()), -2, 4);
    light_block(CRGB::Red, tens(now.hour()), -2, 8);

    light_block(CRGB::Yellow, hundreds(now.year()), 1, 0);
    light_block(CRGB::Yellow, units(now.day()), 1, 4);
    light_block(CRGB::Yellow, units(now.hour()), 1, 8);

    light_block(CRGB::Green, tens(now.year()), 4, 0);
    light_block(CRGB::Green, tens(now.month()), 4, 4);
    light_block(CRGB::Green, tens(now.minute()), 4, 8);

    light_block(CRGB::Blue, units(now.year()), 7, 0);
    light_block(CRGB::Blue, units(now.month()), 7, 4);
    light_block(CRGB::Blue, units(now.minute()), 7, 8);

    FastLED.show();
  }

  sprintf(print_buf, "%4d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  Serial.println(print_buf);
  
  then = now;
}
