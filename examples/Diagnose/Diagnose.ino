// [ru] AlashRelay: реле не щёлкает или работает наоборот? Скетч трижды включит и выключит реле и подскажет, что проверить.
//      Нагрузку на время проверки не подключайте.
// [kk] AlashRelay: реле шықылдамай ма немесе керісінше істей ме? Скетч релені үш рет қосып-өшіреді және нені тексеру керегін айтады.
//      Тексеру кезінде жүктемені қоспаңыз.
// [en] AlashRelay: the relay does not click or works the opposite way? The sketch switches the relay on and off three times and tells what to check.
//      Do not connect a load during the check.
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
