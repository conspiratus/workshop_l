# Установка среды разработки

Эта инструкция описывает установку Visual Studio Code, PlatformIO IDE и Codex,
а также создание нового проекта для платы, используемой в этом репозитории.

## 1. Установка Visual Studio Code

1. Откройте официальную страницу загрузки:
   <https://code.visualstudio.com/download>
2. Скачайте версию для своей операционной системы.
3. Установите Visual Studio Code:
   - Windows: запустите установщик `.exe`;
   - macOS: откройте `.dmg` и перенесите Visual Studio Code в `Applications`;
   - Linux: установите пакет `.deb`, `.rpm` или используйте Snap.
4. Запустите Visual Studio Code.

На macOS команду `code` для терминала можно включить через палитру команд:

1. Нажмите `Cmd+Shift+P`.
2. Выполните `Shell Command: Install 'code' command in PATH`.

Официальная документация:
<https://code.visualstudio.com/docs/getstarted/overview>

## 2. Установка PlatformIO IDE

Отдельно устанавливать PlatformIO Core не требуется: он входит в расширение
PlatformIO IDE.

1. Откройте раздел расширений VS Code:
   - Windows/Linux: `Ctrl+Shift+X`;
   - macOS: `Cmd+Shift+X`.
2. Найдите расширение `PlatformIO IDE`.
3. Проверьте, что издатель расширения — `PlatformIO`.
4. Нажмите `Install`.
5. После установки перезапустите VS Code, если PlatformIO не появился сразу.

В нижней панели VS Code должна появиться кнопка PlatformIO Home.

Официальная документация:
<https://docs.platformio.org/en/latest/integration/ide/vscode.html>

## 3. Установка Codex

1. Откройте раздел расширений VS Code.
2. Найдите расширение `Codex`.
3. Проверьте, что издатель расширения — `OpenAI`.
4. Нажмите `Install`.
5. Если значок Codex не появился, перезапустите VS Code.
6. Откройте Codex в боковой панели.
7. Войдите через аккаунт ChatGPT или укажите API key.

Codex может использовать открытые файлы и выделенный код как контекст, читать и
изменять файлы проекта, запускать сборку и анализировать ошибки.

Официальная документация:
<https://developers.openai.com/codex/ide>

## 4. Создание проекта PlatformIO

1. Откройте PlatformIO Home.
2. Нажмите `New Project`.
3. Укажите имя проекта.
4. В поле `Board` найдите и выберите:

   ```text
   Ai-Thinker ESP-C3-M1-I-Kit
   ```

5. В поле `Framework` выберите:

   ```text
   Arduino
   ```

6. Выберите папку проекта или оставьте стандартное расположение.
7. Нажмите `Finish`.

PlatformIO скачает платформу Espressif, Arduino framework и необходимые
инструменты сборки. Первая настройка может занять несколько минут.

Идентификатор платы в PlatformIO:

```text
esp32-c3-m1i-kit
```

Официальная страница платы:
<https://docs.platformio.org/en/latest/boards/espressif32/esp32-c3-m1i-kit.html>

## 5. Конфигурация platformio.ini

Для этой платы базовая конфигурация выглядит так:

```ini
[env:esp32-c3-m1i-kit]
platform = espressif32
board = esp32-c3-m1i-kit
framework = arduino
monitor_speed = 115200
build_flags =
  -D ARDUINO_USB_CDC_ON_BOOT=1
```

Параметр `ARDUINO_USB_CDC_ON_BOOT=1` включает Serial Monitor через встроенный
USB CDC интерфейс ESP32-C3.

Библиотеки проекта добавляются через `lib_deps`. Например, для LCD1602:

```ini
lib_deps =
  marcoschwartz/LiquidCrystal_I2C@^1.1.4
```

## 6. Структура проекта

После создания проекта основные файлы находятся здесь:

```text
project/
├── platformio.ini
├── include/
├── lib/
├── src/
│   └── main.cpp
└── test/
```

- `platformio.ini` — плата, framework, скорость Serial и зависимости.
- `src/main.cpp` — основной код прошивки.
- `lib/` — локальные библиотеки проекта.
- `test/` — тесты PlatformIO.

Минимальный файл `src/main.cpp`:

```cpp
#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("ESP32-C3 started");
}

void loop() {
  delay(1000);
  Serial.println("Alive");
}
```

## 7. Сборка, прошивка и Serial Monitor

Команды можно запускать через панель PlatformIO или встроенный терминал VS Code.

Сборка:

```sh
pio run
```

Прошивка:

```sh
pio run -t upload
```

Serial Monitor:

```sh
pio device monitor
```

Сборка, прошивка и запуск монитора:

```sh
pio run -t upload && pio device monitor
```

Для выхода из Serial Monitor нажмите `Ctrl+C`.

Если порт не определяется автоматически, укажите его явно:

```sh
pio run -t upload --upload-port /dev/cu.usbmodemXXXX
pio device monitor --port /dev/cu.usbmodemXXXX
```

В Windows порт обычно имеет вид `COM3`, `COM4` и так далее.

## 8. Открытие существующего проекта

Для работы с этим репозиторием новый проект создавать не нужно:

1. Откройте VS Code.
2. Выберите `File -> Open Folder`.
3. Откройте корневую папку репозитория.
4. Дождитесь загрузки PlatformIO.
5. Откройте `platformio.ini` и `src/main.cpp`.
6. Используйте команды `Build`, `Upload` и `Monitor` из панели PlatformIO.

В этом репозитории отдельные примеры находятся в папке `workshops/`. Каждый
workshop может иметь собственный файл `platformio.ini`.
