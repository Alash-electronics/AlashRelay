// [ru] AlashRelay: реле по таймеру без delay(). Нажатие кнопки на пине 4 (второй контакт на GND) включает реле на 5 секунд
//      (как автоматическая лампа в подъезде), потом оно само выключается. Программа всё время свободна.
// [kk] AlashRelay: delay() жоқ таймердегі реле. 4 пиндегі батырманы басу релені 5 секундқа қосады
//      (кіреберістегі автоматты шам сияқты), сосын ол өзі өшеді. Бағдарлама барлық уақытта бос.
// [en] AlashRelay: a relay on a timer without delay(). A button press on pin 4 (the other contact to GND) switches the relay on for 5 seconds
//      (like an automatic lamp in a stairwell), then it switches off by itself. The program stays free all the time.
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
