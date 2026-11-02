// [ru] AlashRelay: термостат с защитой. Реле греет, пока температура ниже порога. setMinSwitchMs(30000) не даёт реле щёлкать чаще
//      раза в 30 секунд (контакты и нагреватель живут дольше), setMaxOnMs() выключит нагрев, если он включён слишком долго.
//      Температура с аналогового датчика LM35 на пине A0 (10 мВ на градус).
// [kk] AlashRelay: қорғаныстағы термостат. Температура шектен төмен болғанда реле қыздырады. setMinSwitchMs(30000) релені 30 секундта бір реттен жиі
//      шықылдатпайды (түйіспелер мен қыздырғыш ұзағырақ қызмет етеді), setMaxOnMs() қыздыру тым ұзақ қосулы тұрса өшіреді.
//      Температура A0 пініндегі LM35 аналогтық датчигінен (градусына 10 мВ).
// [en] AlashRelay: a thermostat with protection. The relay heats while the temperature is below the threshold. setMinSwitchMs(30000) keeps the relay from clicking more often
//      than once in 30 seconds (the contacts and the heater last longer), setMaxOnMs() switches the heating off if it has been on too long.
//      The temperature from an analog LM35 sensor on pin A0 (10 mV per degree).
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
