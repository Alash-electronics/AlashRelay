// AlashRelay: реле по таймеру без delay(). Нажатие кнопки на пине 4 (второй контакт на GND) включает реле на 5 секунд
// (как автоматическая лампа в подъезде), потом оно само выключается. Программа всё время свободна.
#include <AlashRelay.h>

AlashRelay relay(7);

void setup() {
  pinMode(4, INPUT_PULLUP);
  relay.begin();
}

void loop() {
  relay.update();                          // обязателен: считает таймеры
  if (digitalRead(4) == LOW && !relay.isBusy()) {
    relay.pulse(5000);                     // включить на 5 секунд
  }
}
