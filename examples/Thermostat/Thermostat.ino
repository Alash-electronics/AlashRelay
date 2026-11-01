// AlashRelay: термостат с защитой. Реле греет, пока температура ниже порога. setMinSwitchMs(30000) не даёт реле щёлкать чаще
// раза в 30 секунд (контакты и нагреватель живут дольше), setMaxOnMs() выключит нагрев, если он включён слишком долго.
// Температура с аналогового датчика LM35 на пине A0 (10 мВ на градус).
#include <AlashRelay.h>

AlashRelay heater(7);
const float TARGET = 25.0;
const float HYSTERESIS = 1.0;

void setup() {
  Serial.begin(115200);
  heater.begin();
  heater.setMinSwitchMs(30000);            // не чаще раза в 30 секунд
  heater.setMaxOnMs(30UL * 60UL * 1000UL); // не дольше 30 минут подряд
}

void loop() {
  heater.update();
  float t = analogRead(A0) * 5.0 / 1023.0 * 100.0;
  if (t < TARGET - HYSTERESIS) heater.on();
  if (t > TARGET + HYSTERESIS) heater.off();
  Serial.print(t, 1);
  Serial.println(heater.isOn() ? F(" C  нагрев включён") : F(" C  нагрев выключен"));
  if (heater.error() == AlashRelay::ERR_MAX_ON) Serial.println(heater.errorText());
  delay(1000);
}
