# AlashRelay

[Русский](#русский) | [Қазақша](#қазақша) | [English](#english)

---

## Русский

Простая и **безопасная** библиотека для **модуля реле**: не щёлкает при запуске платы, понимает модули с активным LOW, делает импульсы и таймеры без `delay()`, защищает реле от слишком частых переключений и от слишком долгой работы, а два реле для реверса мотора не даёт включить вместе. Без зависимостей. Сделана для уроков и наборов Alash Electronics.

```cpp
#include <AlashRelay.h>
AlashRelay relay(7);

void setup() { relay.begin(); }
void loop() {
  relay.on();  delay(2000);
  relay.off(); delay(2000);
}
```

### Что умеет
- **Без щелчка при старте:** `begin()` сначала задаёт уровень «выключено», потом включает выход. Это важно для нагрузки, которая не должна дёрнуться при перезагрузке.
- **Любой модуль:** активный LOW (по умолчанию) или HIGH: `AlashRelay relay(7, false)`.
- **Таймеры без delay():** `pulse(5000)`, `onAfter()`, `offAfter()`, `offFor()` и `update()` в `loop()`.
- **Защита:** `setMinSwitchMs()` (не чаще раза в N мс), `setMaxOnMs()` (само выключится, например, насос через минуту).
- **Реверс мотора:** `forward.exclusiveWith(backward, 100)`: два реле никогда не включены вместе, между ними пауза.
- **Ресурс:** `switchCount()` и `onTimeMs()` показывают, сколько работало реле.
- **Понятные подсказки** на трёх языках: `errorText()`, `diagnose(Serial)` трижды щёлкнет реле и скажет, что проверить.
- Работает на **Arduino AVR, ESP32, ESP8266**.

### Подключение
| Модуль реле | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В | 5 В (VIN) |
| GND | GND | GND |
| IN | D7 | GPIO26 |
| COM / NO | нагрузка (замыкается при включении) | то же |

**Осторожно:** сеть 220 В опасна для жизни. На уроках используйте низкое напряжение. Мотор или реле нельзя питать от пина 5V платы при большом токе, нужен отдельный источник. Многие модули реле на 5 В работают от 3.3 В ESP32 нестабильно: используйте модуль с оптопарой или транзистор.

### Чем отличается от других библиотек
Мы посмотрели Relay (ArminJo), ezRelay, Relay-Control и примеры из курсов:
- В примерах реле «щёлкает» при каждом старте платы, потому что `pinMode(OUTPUT)` включает выход с низким уровнем (для активного LOW это «включено»). У нас порядок исправлен.
- Таймеры сделаны через `delay()`; у нас `pulse()` и `update()`.
- Защиты от частых переключений, предела по времени и взаимной блокировки нет; у нас есть.

### Примеры
`Basic`, `Timed`, `Interlock`, `Thermostat`, `Diagnose`

### Тесты
Порядок «уровень до OUTPUT» (нет щелчка), уровни для модулей LOW и HIGH, импульсы и отложенные команды, защита от частых переключений, предел времени включения, взаимная блокировка с паузой, счётчики проверены без железа по журналу вызовов: `extras/test`.

Подробное руководство: [docs/WIKI.md](docs/WIKI.md).

---

## Қазақша

**Реле модуліне** арналған қарапайым және **қауіпсіз** кітапхана: тақта қосылғанда шырт етпейді, активті LOW модульдерін түсінеді, `delay()` жоқ импульстер мен таймерлер жасайды, релені тым жиі ауыстырудан және тым ұзақ жұмыстан қорғайды. Тәуелділіксіз.

```cpp
#include <AlashRelay.h>
AlashRelay relay(7);

void setup() { relay.begin(); }
void loop() {
  relay.on();  delay(2000);
  relay.off(); delay(2000);
}
```

### Мүмкіндіктері
- **Қосылғанда шырт етпейді:** `begin()` алдымен «өшірілген» деңгейді қояды.
- **Кез келген модуль:** активті LOW (әдепкі) не HIGH.
- **delay() жоқ таймерлер:** `pulse(5000)`, `onAfter()`, `offAfter()` және `update()`.
- **Қорғаныс:** `setMinSwitchMs()`, `setMaxOnMs()`.
- **Мотор реверсі:** `forward.exclusiveWith(backward, 100)`.
- **Ресурс:** `switchCount()`, `onTimeMs()`.
- **Түсінікті кеңестер** үш тілде.
- **Arduino AVR, ESP32, ESP8266**.

### Қосу
| Реле модулі | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В | 5 В (VIN) |
| GND | GND | GND |
| IN | D7 | GPIO26 |
| COM / NO | нагрузка (замыкается при включении) | то же |

**Абайлаңыз:** 220 В желісі өмірге қауіпті. Сабақта төмен кернеуді қолданыңыз.

### Басқа кітапханалардан айырмашылығы
Relay (ArminJo), ezRelay, Relay-Control қарадық:
- Мысалдарда реле әр қосылғанда шырт етеді; бізде реті түзетілген.
- Таймерлер `delay()` арқылы; бізде `pulse()` және `update()`.
- Қорғаныс пен өзара бұғаттау бізде бар.

### Мысалдар
`Basic`, `Timed`, `Interlock`, `Thermostat`, `Diagnose`

### Тесттер
Деңгей OUTPUT-тан бұрын (шырт жоқ), LOW және HIGH модульдері, импульстер, қорғаныс, өзара бұғаттау, есептегіштер тексерілген: `extras/test`.

Толық нұсқаулық: [docs/WIKI.md](docs/WIKI.md).

---

## English

A simple and **safe** library for the **relay module**: no click when the board starts, understands active-LOW modules, makes pulses and timers without `delay()`, protects the relay from switching too often or running too long, and never lets two motor-reversing relays be on together. No dependencies. Made for classes and Alash Electronics kits.

```cpp
#include <AlashRelay.h>
AlashRelay relay(7);

void setup() { relay.begin(); }
void loop() {
  relay.on();  delay(2000);
  relay.off(); delay(2000);
}
```

### Features
- **No click at start:** `begin()` sets the off level first and enables the pin afterwards. This matters for loads that must not jerk on a reset.
- **Any module:** active LOW (default) or HIGH: `AlashRelay relay(7, false)`.
- **Timers without delay():** `pulse(5000)`, `onAfter()`, `offAfter()`, `offFor()` and `update()` in `loop()`.
- **Protection:** `setMinSwitchMs()` (no faster than N ms), `setMaxOnMs()` (switches itself off, for example a pump after a minute).
- **Motor reversing:** `forward.exclusiveWith(backward, 100)`: two relays are never on together, with a dead time between them.
- **Wear:** `switchCount()` and `onTimeMs()` show how much the relay worked.
- **Clear tips** in three languages: `errorText()`, `diagnose(Serial)` clicks the relay three times and says what to check.
- Works on **Arduino AVR, ESP32, ESP8266**.

### Wiring
| Relay module | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В | 5 В (VIN) |
| GND | GND | GND |
| IN | D7 | GPIO26 |
| COM / NO | нагрузка (замыкается при включении) | то же |

**Warning:** 220 V mains is dangerous. Use low voltage in class. A motor or coil must not be powered from the board 5V pin at high current; use a separate supply. Many 5 V relay modules are unreliable from a 3.3 V ESP32: use a module with an optocoupler or a transistor.

### How it differs from other libraries
We looked at Relay (ArminJo), ezRelay, Relay-Control and course examples:
- In examples the relay "clicks" on every board start because `pinMode(OUTPUT)` drives the pin LOW (which is "on" for active-LOW modules). We fixed the order.
- Timers use `delay()`; we have `pulse()` and `update()`.
- No protection against fast switching, no on-time limit and no interlock; we have all three.

### Examples
`Basic`, `Timed`, `Interlock`, `Thermostat`, `Diagnose`

### Tests
The "level before OUTPUT" order (no click), levels for LOW and HIGH modules, pulses and delayed commands, fast-switching protection, the on-time limit, the interlock with dead time and the counters are checked without hardware from the call log: `extras/test`.

Full guide: [docs/WIKI.md](docs/WIKI.md).

---

MIT License, Alash Electronics.
