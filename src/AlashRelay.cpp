// MIT License. Alash Electronics.
#include "AlashRelay.h"

#define T(ru, kk, en) (_lang == LANG_KK ? F(kk) : _lang == LANG_EN ? F(en) : F(ru))

void AlashRelay::begin(bool initialOn) {
  _on = initialOn;
  digitalWrite(_pin, ((_on != _low)) ? HIGH : LOW);   // уровень до включения выхода: у реле при запуске нет щелчка
  pinMode(_pin, OUTPUT);
  _started = true;
  _onSince = millis();
}

void AlashRelay::write(bool on) {
  if (!_started) begin(false);
  uint32_t now = millis();
  if (on && !_on) {
    _count++;
    _onSince = now;
  } else if (!on && _on) {
    _onTotal += now - _onSince;
  }
  _on = on;
  _lastSwitch = now;
  _hasSwitched = true;
  digitalWrite(_pin, (on != _low) ? HIGH : LOW);
}

// Слишком частое переключение запрещено, если задан setMinSwitchMs
bool AlashRelay::allowed() {
  if (_minSwitch && _hasSwitched && (uint32_t)(millis() - _lastSwitch) < _minSwitch) {
    _err = ERR_TOO_FAST;
    return false;
  }
  return true;
}

void AlashRelay::on() {
  if (!_started) begin(false);
  if (_on && !_pendingOn) return;
  if (!allowed()) return;
  if (_other && _other->_on) {                // напарник включён: сначала выключаем его и ждём паузу, потом включаемся сами
    _other->write(false);
    _pendingOn = true;
    _pendingAt = millis() + _dead;
    return;
  }
  _err = ERR_OK;
  write(true);
}

void AlashRelay::off() {
  if (!_started) begin(false);
  _pendingOn = false;
  if (!_on) return;
  if (!allowed()) return;
  _err = ERR_OK;
  write(false);
}

void AlashRelay::toggle() { if (_on) off(); else on(); }
void AlashRelay::set(bool state) { if (state) on(); else off(); }

void AlashRelay::pulse(uint32_t ms) {
  on();
  _timerMode = OFF_AT;
  _timerAt = millis() + ms;
}

void AlashRelay::offFor(uint32_t ms) {
  off();
  _timerMode = ON_AT;
  _timerAt = millis() + ms;
}

void AlashRelay::onAfter(uint32_t ms) { _timerMode = ON_AT; _timerAt = millis() + ms; }
void AlashRelay::offAfter(uint32_t ms) { _timerMode = OFF_AT; _timerAt = millis() + ms; }
void AlashRelay::cancel() { _timerMode = NONE; _pendingOn = false; }

uint32_t AlashRelay::onTimeMs() { return _onTotal + (_on ? (uint32_t)(millis() - _onSince) : 0); }

void AlashRelay::update() {
  uint32_t now = millis();
  if (_pendingOn && (int32_t)(now - _pendingAt) >= 0) {
    _pendingOn = false;
    write(true);
  }
  if (_timerMode != NONE && (int32_t)(now - _timerAt) >= 0) {
    Timer m = _timerMode;
    _timerMode = NONE;
    if (m == OFF_AT) off(); else if (m == ON_AT) on();
  }
  if (_maxOn && _on && (uint32_t)(now - _onSince) >= _maxOn) {   // аварийное выключение по времени
    _err = ERR_MAX_ON;
    _timerMode = NONE;
    write(false);
  }
}

void AlashRelay::exclusiveWith(AlashRelay &other, uint16_t deadMs) {
  _other = &other;
  other._other = this;
  _dead = other._dead = deadMs;
}

const __FlashStringHelper *AlashRelay::errorText() {
  switch (_err) {
    case ERR_OK: return F("OK");
    case ERR_TOO_FAST:
      return T("Переключение отклонено: прошло меньше времени, чем задано в setMinSwitchMs() (защита реле)",
               "Ауыстыру қабылданбады: setMinSwitchMs() уақытынан аз өтті (реле қорғанысы)",
               "Switching refused: less time passed than set in setMinSwitchMs() (relay protection)");
    case ERR_MAX_ON:
      return T("Реле выключено автоматически: оно было включено дольше времени, заданного в setMaxOnMs()",
               "Реле автоматты түрде өшірілді: setMaxOnMs() уақытынан ұзақ қосулы болды",
               "The relay was switched off automatically: it was on longer than set in setMaxOnMs()");
  }
  return F("");
}

void AlashRelay::diagnose(Print &out) {
  out.println(T("AlashRelay: диагностика", "AlashRelay: диагностика", "AlashRelay: diagnostics"));
  out.print(T("Пин: ", "Пин: ", "Pin: "));
  out.println((int)_pin);
  out.println(_low ? T("Уровень включения: LOW (так у большинства модулей)", "Қосу деңгейі: LOW", "Active level: LOW (most modules)")
                   : T("Уровень включения: HIGH", "Қосу деңгейі: HIGH", "Active level: HIGH"));
  out.println(T("Сейчас реле трижды включится и выключится (слышны щелчки, горит светодиод на модуле). Если не щёлкает или щёлкает наоборот: поменяйте activeLow (AlashRelay r(пин, false)); проверьте питание модуля 5 В (VCC и GND) и общий GND с платой. Не подключайте нагрузку 220 В на время проверки.",
                "Қазір реле үш рет қосылып өшеді (шырт дыбысы, модульдегі жарықдиод). Шырт етпесе не кері болса: activeLow өзгертіңіз (AlashRelay r(пин, false)); модуль қуатын 5 В және ортақ GND тексеріңіз. Тексеру кезінде 220 В жүктемені қоспаңыз.",
                "The relay will click on and off three times now (you hear clicks, the module LED lights). If it does not click or works inverted: change activeLow (AlashRelay r(pin, false)); check the 5 V module power and a common GND. Do not connect a 220 V load during the test."));
  for (uint8_t i = 0; i < 3; i++) {
    on(); delay(400);
    off(); delay(400);
  }
}
