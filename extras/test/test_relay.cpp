// Тесты AlashRelay без железа: журнал записей на пин, шаг времени по 1 мс.
//   g++ -std=c++17 -Wall -I mock -I ../../src test_relay.cpp ../../src/AlashRelay.cpp && ./a.out
#include <stdio.h>
#include "Arduino.h"
#include "AlashRelay.h"

static int fails = 0;
#define CHECK(cond) do { if (!(cond)) { printf("FAIL line %d: %s\n", __LINE__, #cond); fails++; } } while (0)
static void reset() { g_calls.clear(); g_ms = 1000; memset(g_level, 0, sizeof g_level); }
static void run(AlashRelay &r, uint32_t ms) { for (uint32_t i = 0; i < ms; i++) { g_ms++; r.update(); } }
static void run2(AlashRelay &a, AlashRelay &b, uint32_t ms) { for (uint32_t i = 0; i < ms; i++) { g_ms++; a.update(); b.update(); } }

int main() {
  // 1) без щелчка при старте: уровень «выключено» записан ДО pinMode(OUTPUT); активный LOW => HIGH
  reset(); AlashRelay r(5);
  r.begin();
  CHECK(g_calls.size() == 2 && g_calls[0].kind == 'W' && g_calls[0].value == HIGH && g_calls[1].kind == 'M' && g_calls[1].value == OUTPUT);
  reset(); AlashRelay hi(5, false); hi.begin(); CHECK(g_calls[0].kind == 'W' && g_calls[0].value == LOW);   // активный HIGH: выкл = LOW
  reset(); AlashRelay si(5); si.begin(true); CHECK(g_calls[0].value == LOW && si.isOn());                    // включено при старте

  // 2) включить / выключить / переключить; уровни для обоих типов модулей
  reset(); AlashRelay a(6);
  a.on();  CHECK(a.isOn() && g_level[6] == LOW);
  a.off(); CHECK(!a.isOn() && g_level[6] == HIGH);
  a.toggle(); CHECK(a.isOn()); a.toggle(); CHECK(!a.isOn());
  a.set(true); CHECK(a.isOn()); a.set(false); CHECK(!a.isOn());
  reset(); AlashRelay h(6, false); h.on(); CHECK(g_level[6] == HIGH); h.off(); CHECK(g_level[6] == LOW);

  // 3) импульс и временные команды
  reset(); AlashRelay p(7);
  p.pulse(200); CHECK(p.isOn() && p.isBusy());
  run(p, 150); CHECK(p.isOn()); run(p, 60); CHECK(!p.isOn() && !p.isBusy());
  p.onAfter(300); CHECK(!p.isOn()); run(p, 290); CHECK(!p.isOn()); run(p, 20); CHECK(p.isOn());
  p.offAfter(100); run(p, 110); CHECK(!p.isOn());
  p.on(); p.offFor(100); CHECK(!p.isOn()); run(p, 110); CHECK(p.isOn());
  p.cancel(); p.off();
  p.onAfter(100); p.cancel(); run(p, 200); CHECK(!p.isOn());

  // 4) защита от частых переключений
  reset(); AlashRelay m(8); m.setMinSwitchMs(1000);
  m.on(); CHECK(m.isOn());
  g_ms += 200; m.off(); CHECK(m.isOn() && m.error() == AlashRelay::ERR_TOO_FAST);     // слишком рано
  g_ms += 900; m.off(); CHECK(!m.isOn() && m.error() == AlashRelay::ERR_OK);

  // 5) предельное время включения (насос, нагреватель)
  reset(); AlashRelay pump(9); pump.setMaxOnMs(5000);
  pump.on(); run(pump, 4900); CHECK(pump.isOn());
  run(pump, 200); CHECK(!pump.isOn() && pump.error() == AlashRelay::ERR_MAX_ON);

  // 6) взаимная блокировка (реверс мотора): два реле никогда не включены одновременно, между ними пауза
  reset(); AlashRelay fwd(10), rev(11); fwd.exclusiveWith(rev, 100);
  fwd.on(); CHECK(fwd.isOn() && !rev.isOn());
  rev.on();                                                    // просит включиться: fwd выключается, rev ждёт
  CHECK(!fwd.isOn() && !rev.isOn());
  bool bothOn = false;
  for (int i = 0; i < 300; i++) { g_ms++; fwd.update(); rev.update(); if (fwd.isOn() && rev.isOn()) bothOn = true; }
  CHECK(!bothOn && rev.isOn() && !fwd.isOn());
  // момент включения rev был не раньше чем через 100 мс после выключения fwd
  fwd.on(); CHECK(!rev.isOn() && !fwd.isOn());
  run2(fwd, rev, 150); CHECK(fwd.isOn() && !rev.isOn());

  // 7) счётчик включений и время работы
  reset(); AlashRelay c(12); c.begin();
  for (int i = 0; i < 5; i++) { c.on(); g_ms += 100; c.off(); g_ms += 50; }
  CHECK(c.switchCount() == 5 && c.onTimeMs() == 500);
  c.on(); g_ms += 70; CHECK(c.onTimeMs() == 570);

  // 8) диагностика
  reset(); AlashRelay d(13); d.setLanguage(AlashRelay::LANG_KK); const char *kk = (const char *)d.errorText();
  d.setLanguage(AlashRelay::LANG_EN); d.diagnose(*(new Print));
  CHECK(kk != nullptr && d.switchCount() == 3);

  if (fails) { printf("FAILED: %d\n", fails); return 1; }
  printf("ALL TESTS PASSED\n");
  return 0;
}
