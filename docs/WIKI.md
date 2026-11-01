# AlashRelay: руководство / нұсқаулық / guide

[Русский](#русский) | [Қазақша](#қазақша) | [English](#english)

---

## Русский

Простая и **безопасная** библиотека для **модуля реле**: не щёлкает при запуске платы, понимает модули с активным LOW, делает импульсы и таймеры без `delay()`, защищает реле от слишком частых переключений и от слишком долгой работы, а два реле для реверса мотора не даёт включить вместе. Без зависимостей. Сделана для уроков и наборов Alash Electronics.

### Установка
Arduino IDE: Скетч, Подключить библиотеку, Управлять библиотеками, найти **AlashRelay** и нажать Установить. Или скачайте ZIP с GitHub: Скетч, Подключить библиотеку, Добавить .ZIP библиотеку.

### Платы
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 и другие, где работает Arduino-ядро.

### Подключение
| Модуль реле | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В | 5 В (VIN) |
| GND | GND | GND |
| IN | D7 | GPIO26 |
| COM / NO | нагрузка (замыкается при включении) | то же |

**Осторожно:** сеть 220 В опасна для жизни. На уроках используйте низкое напряжение. Мотор или реле нельзя питать от пина 5V платы при большом токе, нужен отдельный источник. Многие модули реле на 5 В работают от 3.3 В ESP32 нестабильно: используйте модуль с оптопарой или транзистор.

### Быстрый старт
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

### Справочник по методам
| Метод | Что делает |
|---|---|
| `begin(initialOn)` | Безопасно настроить пин |
| `on() / off() / toggle() / set(bool)` | Управление |
| `pulse(ms) / onAfter(ms) / offAfter(ms) / offFor(ms)` | Таймеры (нужен update()) |
| `update()` | Вызывать в loop() |
| `setMinSwitchMs(ms)` | Не переключать чаще |
| `setMaxOnMs(ms)` | Само выключится через ms |
| `exclusiveWith(other, deadMs)` | Взаимная блокировка двух реле |
| `switchCount() / onTimeMs()` | Ресурс контактов |
| `diagnose(Serial)` | Щёлкнуть трижды и подсказать |

### Если что-то не работает
**Реле работает наоборот**

Модуль с активным HIGH: `AlashRelay relay(7, false)`. Большинство дешёвых модулей включаются уровнем LOW (это значение по умолчанию).

**Реле щёлкает, но нагрузка не включается**

Нагрузка подключена к COM и NC (нормально замкнутый) вместо NO. Для включения при команде используйте COM и NO.

**ESP32 перезагружается при включении реле**

Катушка реле просаживает питание. Запитайте модуль отдельно (общий GND) или поставьте конденсатор 470-1000 мкФ у модуля.

### Самопроверка
В библиотеке есть `diagnose(Serial)`: она печатает, что видит плата, и подсказывает, что проверить. Язык подсказок: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Примеры
`Basic`, `Timed`, `Interlock`, `Thermostat`, `Diagnose`

---

## Қазақша

**Реле модуліне** арналған қарапайым және **қауіпсіз** кітапхана: тақта қосылғанда шырт етпейді, активті LOW модульдерін түсінеді, `delay()` жоқ импульстер мен таймерлер жасайды, релені тым жиі ауыстырудан және тым ұзақ жұмыстан қорғайды. Тәуелділіксіз.

### Орнату
Arduino IDE: Sketch, Include Library, Manage Libraries, **AlashRelay** табып, Install басыңыз. Немесе GitHub-тан ZIP жүктеп, Sketch, Include Library, Add .ZIP Library.

### Тақталар
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 және Arduino ядросы бар басқалар.

### Қосу
| Реле модулі | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В | 5 В (VIN) |
| GND | GND | GND |
| IN | D7 | GPIO26 |
| COM / NO | нагрузка (замыкается при включении) | то же |

**Абайлаңыз:** 220 В желісі өмірге қауіпті. Сабақта төмен кернеуді қолданыңыз.

### Жылдам бастау
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

### Әдістер анықтамасы
| Әдіс | Не істейді |
|---|---|
| `begin(initialOn)` | Пинді қауіпсіз баптау |
| `on() / off() / toggle() / set(bool)` | Басқару |
| `pulse(ms) / onAfter(ms) / offAfter(ms) / offFor(ms)` | Таймерлер |
| `update()` | loop() ішінде шақыру |
| `setMinSwitchMs(ms)` | Жиі ауыстырмау |
| `setMaxOnMs(ms)` | ms кейін өзі өшеді |
| `exclusiveWith(other, deadMs)` | Екі релені бұғаттау |
| `switchCount() / onTimeMs()` | Контакт ресурсы |
| `diagnose(Serial)` | Үш рет шырт ету |

### Бірдеңе жұмыс істемесе
**Реле кері жұмыс істейді**

Белсенді HIGH модулі: `AlashRelay relay(7, false)`.

**Реле шырт етеді, бірақ жүктеме қосылмайды**

Жүктеме COM және NO-ға қосылуы керек.

**Реле қосылғанда ESP32 қайта қосылады**

Модульді бөлек қоректендіріңіз.

### Өзін-өзі тексеру
Кітапханада `diagnose(Serial)` бар: тақта не көргенін басып шығарады және нені тексеру керегін айтады. Кеңес тілі: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Мысалдар
`Basic`, `Timed`, `Interlock`, `Thermostat`, `Diagnose`

---

## English

A simple and **safe** library for the **relay module**: no click when the board starts, understands active-LOW modules, makes pulses and timers without `delay()`, protects the relay from switching too often or running too long, and never lets two motor-reversing relays be on together. No dependencies. Made for classes and Alash Electronics kits.

### Installation
Arduino IDE: Sketch, Include Library, Manage Libraries, search **AlashRelay** and click Install. Or download the ZIP from GitHub: Sketch, Include Library, Add .ZIP Library.

### Boards
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 and others with an Arduino core.

### Wiring
| Relay module | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В | 5 В (VIN) |
| GND | GND | GND |
| IN | D7 | GPIO26 |
| COM / NO | нагрузка (замыкается при включении) | то же |

**Warning:** 220 V mains is dangerous. Use low voltage in class. A motor or coil must not be powered from the board 5V pin at high current; use a separate supply. Many 5 V relay modules are unreliable from a 3.3 V ESP32: use a module with an optocoupler or a transistor.

### Quick start
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

### Method reference
| Method | What it does |
|---|---|
| `begin(initialOn)` | Safely set up the pin |
| `on() / off() / toggle() / set(bool)` | Control |
| `pulse(ms) / onAfter(ms) / offAfter(ms) / offFor(ms)` | Timers (need update()) |
| `update()` | Call in loop() |
| `setMinSwitchMs(ms)` | Do not switch faster |
| `setMaxOnMs(ms)` | Switches itself off after ms |
| `exclusiveWith(other, deadMs)` | Interlock of two relays |
| `switchCount() / onTimeMs()` | Contact wear |
| `diagnose(Serial)` | Click three times and give tips |

### If something does not work
**The relay works inverted**

An active-HIGH module: `AlashRelay relay(7, false)`. Most cheap modules switch on LOW (the default).

**The relay clicks but the load does not switch on**

The load is on COM and NC (normally closed) instead of NO. Use COM and NO to switch on at the command.

**The ESP32 resets when the relay switches on**

The relay coil sags the supply. Power the module separately (common GND) or add a 470-1000 uF capacitor near it.

### Self-check
The library has `diagnose(Serial)`: it prints what the board sees and tells what to check. Tips language: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Examples
`Basic`, `Timed`, `Interlock`, `Thermostat`, `Diagnose`

---

MIT License, Alash Electronics. https://github.com/Alash-electronics/AlashRelay
