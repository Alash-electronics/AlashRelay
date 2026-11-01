// AlashRelay: модуль реле: включение без щелчка при старте, активный LOW, импульсы и таймеры без delay(), защита от частых переключений и взаимная блокировка.
// Без зависимостей. MIT License. Alash Electronics, https://github.com/Alash-electronics/AlashRelay
#ifndef ALASH_RELAY_H
#define ALASH_RELAY_H

#include <Arduino.h>

class AlashRelay {
 public:
  enum Error : uint8_t { ERR_OK = 0, ERR_TOO_FAST, ERR_MAX_ON };
  enum Language : uint8_t { LANG_RU = 0, LANG_KK = 1, LANG_EN = 2 };

  // pin: сигнальный вход модуля. activeLow: большинство модулей включаются уровнем LOW (по умолчанию true).
  explicit AlashRelay(uint8_t pin, bool activeLow = true) : _pin(pin), _low(activeLow) {}

  // begin(): сначала задаёт безопасный уровень «выключено» и только потом включает выход: реле не щёлкает при запуске.
  void begin(bool initialOn = false);

  // ---- Управление ----------------------------------------------------------------
  void on();
  void off();
  void toggle();
  void set(bool state);
  bool isOn() const { return _on; }

  // ---- Без delay() (нужен update() в loop()) --------------------------------------------
  void pulse(uint32_t ms);                  // включить на ms и выключить
  void onFor(uint32_t ms) { pulse(ms); }
  void offFor(uint32_t ms);                 // выключить на ms и включить обратно
  void onAfter(uint32_t ms);                // включить через ms
  void offAfter(uint32_t ms);               // выключить через ms
  void cancel();                            // отменить отложенное
  bool isBusy() const { return _timerMode != NONE; }
  void update();

  // ---- Защита --------------------------------------------------------------------
  void setMinSwitchMs(uint32_t ms) { _minSwitch = ms; }     // не переключать чаще: защищает реле и нагрузку (по умолчанию 0 = без ограничения)
  void setMaxOnMs(uint32_t ms) { _maxOn = ms; }             // сам выключится, если включено слишком долго (насос, нагреватель); 0 = без ограничения
  void exclusiveWith(AlashRelay &other, uint16_t deadMs = 50);   // два реле никогда не включены вместе (реверс мотора): между ними пауза
  uint32_t switchCount() const { return _count; }          // сколько раз реле включилось (ресурс контактов ограничен)
  uint32_t onTimeMs();                                      // сколько всего было включено

  // ---- Диагностика -----------------------------------------------------------------
  Error error() const { return _err; }
  void setLanguage(Language lang) { _lang = lang; }
  const __FlashStringHelper *errorText();
  void diagnose(Print &out);

 private:
  enum Timer : uint8_t { NONE, OFF_AT, ON_AT, PULSE_BACK_OFF, PULSE_BACK_ON };
  uint8_t _pin;
  bool _low, _on = false, _started = false;
  Error _err = ERR_OK;
  Language _lang = LANG_RU;
  uint32_t _minSwitch = 0, _maxOn = 0, _lastSwitch = 0, _onSince = 0, _onTotal = 0, _count = 0;
  bool _hasSwitched = false;
  Timer _timerMode = NONE;
  uint32_t _timerAt = 0;
  AlashRelay *_other = nullptr;
  uint16_t _dead = 50;
  bool _pendingOn = false;                  // ждёт паузы после выключения напарника
  uint32_t _pendingAt = 0;

  void write(bool on);
  bool allowed();
};

#endif  // ALASH_RELAY_H
