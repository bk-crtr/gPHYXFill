# Инструкция по сборке плагина GaussianSplatFX для Motion

## Обзор
Этот документ описывает процесс сборки FxPlug плагина в Xcode и его установки для использования в Apple Motion и Final Cut Pro.

## Требования
- macOS 12.0 или выше
- Xcode (версия 26.0.1 или выше)
- FxPlug SDK установлен в `/Library/Developer/SDKs/FxPlug.sdk`
- Apple Motion или Final Cut Pro

## Структура проекта
Проект использует двухцелевую структуру Xcode:
1. **Wrapper Application** (`GaussianSplatFX_G.app`) - обертка-приложение
2. **XPC Service** (`gSplat XPC Service.pluginkit`) - сам плагин

Плагин имеет трехуровневую иерархию:
```
GaussianSplatFX_G.app
  └── Contents/PlugIns/
      └── gSplat XPC Service.pluginkit
          └── Contents/PlugIns/
              └── gSplat XPC Service.fxplug
```

## Процесс сборки

### 1. Открытие проекта в Xcode
```bash
cd /Users/abuahmad/Projects/GaussianSplatFX_G/GaussianSplatFX_G
open GaussianSplatFX_G.xcodeproj
```

### 2. Сборка в Xcode
1. Выберите схему **"Wrapper Application"** в Xcode
2. Выберите **Product → Build** или нажмите `⌘B`
3. Для Release-сборки: **Product → Archive**

### 3. Автоматические Build-скрипты

При сборке автоматически выполняются следующие скрипты (в порядке выполнения):

#### 3.1. Копирование фреймворков
- **Copy and Code Sign FxPlug.framework** - копирует FxPlug.framework из `/Library/Frameworks/`
- **Copy and Code Sign PluginManager.framework** - копирует PluginManager.framework из `/Library/Developer/Frameworks/`

#### 3.2. Реструктуризация бандла
**Скрипт:** `/Scripts/restructure_fxplug.sh`

Этот скрипт создает правильную трехуровневую структуру FxPlug:
- Создает `.fxplug` бандл внутри `.pluginkit`
- Копирует Info.plist для FxPlug бандла
- Создает симлинки к executable и ресурсам

#### 3.3. Подписание кода
- **CodeSign FxPlug Bundle** - подписывает `.fxplug` бандл
- **CodeSign PlugInKit Bundle** - подписывает `.pluginkit` бандл

### 4. Установка плагина

После успешной сборки используйте скрипт установки:

```bash
cd /Users/abuahmad/Projects/GaussianSplatFX_G
./install_plugin.sh
```

#### Что делает скрипт установки:
1. Удаляет старую версию из `/Applications/gPHYXSplat.app`
2. Копирует новую сборку из `DerivedData` в `/Applications/`
3. Устанавливает правильные права доступа
4. Регистрирует плагин в системе через `pluginkit`

### 5. Важные команды

#### Регистрация плагина вручную:
```bash
pluginkit -a /Applications/gPHYXSplat.app/Contents/PlugIns/*.pluginkit
```

#### Проверка регистрации:
```bash
pluginkit -m -v | grep -i gphyxsplat
```

#### Просмотр логов плагина:
```bash
log stream --predicate 'subsystem contains "com.gadjik.gSplat"' --level debug
```

#### Очистка кеша Motion (при проблемах с видимостью):
```bash
sudo killall -9 Motion
sudo killall -9 pluginkit
pluginkit -r /Applications/gPHYXSplat.app/Contents/PlugIns/*.pluginkit
```

## Важные Bundle Identifiers

- **Wrapper App:** `com.gadjik.gSplat`
- **XPC Service:** `com.gadjik.gSplat.xpc`

## Использование в Motion

После установки:
1. Полностью закройте Motion (если запущен)
2. Запустите Motion заново
3. Плагин должен появиться в **Library → Filters/Effects Browser**
4. Категория плагина: согласно настройкам в Info.plist

## Устранение проблем

### Плагин не появляется в Motion
1. Проверьте регистрацию: `pluginkit -m -v | grep -i gphyxsplat`
2. Проверьте права доступа: `ls -la /Applications/gPHYXSplat.app`
3. Очистите кеш Motion (см. команды выше)
4. Проверьте логи: `log stream --predicate 'subsystem contains "FxPlug"' --level debug`

### Ошибки подписи кода
```bash
sudo codesign --force --deep --sign - /Applications/gPHYXSplat.app
```

### Полная переустановка
```bash
# Удалить все следы
sudo rm -rf /Applications/gPHYXSplat.app
pluginkit -r | grep -i gphyxsplat

# Очистить DerivedData
rm -rf ~/Library/Developer/Xcode/DerivedData/GaussianSplatFX_G-*

# Пересобрать проект
# [открыть в Xcode и выполнить Product → Clean Build Folder]
# [затем Product → Build]

# Установить заново
./install_plugin.sh
```

## Разработка

### Быстрая пересборка и установка
```bash
# В одной команде
xcodebuild -project GaussianSplatFX_G.xcodeproj \
  -scheme "Wrapper Application" \
  -configuration Release \
  clean build && ./install_plugin.sh
```

### Debugging
Для отладки плагина в Motion:
1. В Xcode: **Debug → Attach to Process by PID or Name...**
2. Введите "Motion"
3. Установите breakpoints в коде плагина
4. Запустите Motion и загрузите плагин

---

**Последнее обновление:** 2026-01-09  
**Версия проекта:** GaussianSplatFX_G  
**Bundle ID:** com.gadjik.gSplat
