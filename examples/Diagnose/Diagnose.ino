// AlashRelay: реле не щёлкает или работает наоборот? Скетч трижды включит и выключит реле и подскажет, что проверить.
// Нагрузку на время проверки не подключайте.
#include <AlashRelay.h>

AlashRelay relay(7);                       // если реле работает наоборот: AlashRelay relay(7, false);

void setup() {
  Serial.begin(115200);
  delay(500);
  relay.setLanguage(AlashRelay::LANG_RU);  // LANG_KK: қазақша, LANG_EN: English
  relay.begin();
  relay.diagnose(Serial);
}

void loop() {}
