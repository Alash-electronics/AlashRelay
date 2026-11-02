// [ru] AlashRelay: реверс мотора на двух реле. Реле "вперёд" и "назад" нельзя включать одновременно: это замыкание питания.
//      exclusiveWith() делает это невозможным: при смене направления одно реле выключается, ждёт 100 мс и только потом включается второе.
// [kk] AlashRelay: мотор реверсі екі реле арқылы. «Алға» және «артқа» релелерін бір мезгілде қосуға болмайды: бұл қуаттың қысқа тұйықталуы.
//      exclusiveWith() мұны мүмкін емес етеді: бағыт ауысқанда бір реле өшеді, 100 мс күтеді де, сосын ғана екіншісі қосылады.
// [en] AlashRelay: reversing a motor with two relays. The "forward" and "back" relays must not be switched on together: that is a short circuit of the supply.
//      exclusiveWith() makes it impossible: on a change of direction one relay switches off, waits 100 ms and only then the second one switches on.
#include <AlashRelay.h>

AlashRelay forward(7);
AlashRelay backward(8);

void setup() {
  forward.begin();
  backward.begin();
  forward.exclusiveWith(backward, 100);    // пауза между реле 100 мс
}

void loop() {
  forward.update();
  backward.update();
  static uint32_t last = 0;
  static bool dir = true;
  if (millis() - last > 3000) {            // каждые 3 секунды меняем направление
    last = millis();
    if (dir) forward.on(); else backward.on();
    dir = !dir;
  }
}
