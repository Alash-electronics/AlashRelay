// AlashRelay: реверс мотора на двух реле. Реле "вперёд" и "назад" нельзя включать одновременно: это замыкание питания.
// exclusiveWith() делает это невозможным: при смене направления одно реле выключается, ждёт 100 мс и только потом включается второе.
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
