// Имитация Arduino.h для тестов реле: журнал записей на пин и режимов с порядком вызовов.
#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <vector>
class __FlashStringHelper;
#define F(x) (reinterpret_cast<const __FlashStringHelper *>(x))
#define OUTPUT 1
#define LOW 0
#define HIGH 1
inline uint32_t g_ms = 1000;
inline uint32_t millis() { return g_ms; }
inline void delay(uint32_t ms) { g_ms += ms; }
struct Call { char kind; uint8_t pin; int value; };   // 'W' digitalWrite, 'M' pinMode
inline std::vector<Call> g_calls;
inline uint8_t g_level[32];
inline void pinMode(uint8_t p, uint8_t m) { g_calls.push_back({'M', p, m}); }
inline void digitalWrite(uint8_t p, uint8_t v) { g_calls.push_back({'W', p, v}); g_level[p] = v; }
class Print {
 public:
  void print(const char *s) { printf("%s", s); }
  void print(const __FlashStringHelper *s) { printf("%s", (const char *)s); }
  void print(int v) { printf("%d", v); }
  void println(const char *s = "") { printf("%s\n", s); }
  void println(int v) { printf("%d\n", v); }
  void println(const __FlashStringHelper *s) { printf("%s\n", (const char *)s); }
};
