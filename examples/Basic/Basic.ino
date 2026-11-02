// [ru] AlashRelay: самый простой пример. Реле включается и выключается раз в 2 секунды.
//
//      Подключение модуля реле: VCC к 5 В, GND к GND, IN к пину 7. Нагрузку подключайте к контактам COM и NO (замыкается при включении).
//      ВНИМАНИЕ: работа с сетью 220 В опасна. Для уроков используйте низкое напряжение (лампочка на батарейке, мотор 5-12 В).
// [kk] AlashRelay: ең қарапайым мысал. Реле әр 2 секунд сайын қосылып-өшеді.
//
//      Реле модулін қосу: VCC 5 В-қа, GND GND-ға, IN 7 пинге. Жүктемені COM және NO түйіспелеріне қосыңыз (қосылғанда тұйықталады).
//      НАЗАР АУДАРЫҢЫЗ: 220 В желімен жұмыс қауіпті. Сабақтарда төмен кернеуді қолданыңыз (батареядағы шам, 5-12 В мотор).
// [en] AlashRelay: the simplest example. The relay switches on and off every 2 seconds.
//
//      Relay module wiring: VCC to 5 V, GND to GND, IN to pin 7. Connect the load to the COM and NO contacts (closes when switched on).
//      WARNING: working with 220 V mains is dangerous. For lessons use low voltage (a bulb on a battery, a 5-12 V motor).
#include <AlashRelay.h>

AlashRelay relay(7);                       // большинство модулей включаются уровнем LOW: так и задано по умолчанию

void setup() {
  relay.begin();                           // реле при запуске не щёлкает: сначала задаётся «выключено»
}

void loop() {
  relay.on();
  delay(2000);
  relay.off();
  delay(2000);
}
