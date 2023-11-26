#ifndef __UTIL_H__
#define __UTIL_H__

#include "Arduino.h"

#include "FastLED.h"

#include "stdint.h"

#define for_range(I, LT) for (size_t I = 0; I < LT; ++I)

constexpr uint64_t S_PER_MINUTE{60ULL};
constexpr uint64_t S_PER_HOUR{S_PER_MINUTE * 60ULL};
constexpr uint64_t S_PER_DAY{S_PER_HOUR * 24ULL};
constexpr uint64_t S_PER_YEAR{S_PER_DAY * 365ULL};

constexpr int8_t WIDTH{10};
constexpr int8_t HEIGHT{11};
constexpr int8_t BOTTOM{0};
constexpr int8_t LEFT{0};
constexpr int8_t TOP{HEIGHT - 1};
constexpr int8_t RIGHT{WIDTH - 1};
constexpr uint8_t NUM_LEDS{WIDTH * HEIGHT};

constexpr unsigned char LED_PIN{2};
constexpr uint8_t BRIGHTNESS{34};

constexpr uint64_t period_1s_ms{1000ULL}; // must be at least 1

extern CRGB leds[];

// serpentine layout, origin at bottom left
auto xy = [](int8_t x, int8_t y){ return (x % 2) ? (HEIGHT * (x + 1)) - y - 1 : y + (HEIGHT * x); };

#endif // __UTIL_H__
