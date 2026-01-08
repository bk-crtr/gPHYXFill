---
description: Установка и отладка gPHYX Fill FxPlug плагина v2.20
---

# Установка gPHYX Fill v2.20

## Быстрая Автоматическая Установка

**Самый простой способ** — запустить автоматический скрипт:

```bash
cd /Users/abuahmad/Projects/gPHYXFill
./auto_install.sh
```

Скрипт автоматически:
1. Закроет Motion и Final Cut Pro
2. Удалит старые версии
3. Очистит кэши системы
4. Пересоберёт проект (Release)
5. Установит в `/Applications`
6. Зарегистрирует плагин
7. Откроет Motion

**Плагин появится в**: Motion → Библиотека → Фильтры → **gPHYX 220** → gPHYX Fill v2.20

---

## Критические Требования для Видимости в Motion

⚠️ **ВАЖНО**: Эти параметры в `XPCInfo.plist` обязательны для работы:

### 1. PrincipalClass должен быть стандартным

```xml
<key>PrincipalClass</key>
<string>FxPrincipal</string>  <!-- НЕ gPHYXPrincipal! -->
```

### 2. НЕ должно быть com.apple.version

```xml
<key>Attributes</key>
<dict>
    <key>com.apple.protocol</key>
    <string>FxPlug</string>
    <!-- com.apple.version УДАЛИТЬ! -->
</dict>
```

### 3. Не использовать кастомный Swift Principal

- ❌ Удалить `FxPrincipal.swift` из `project.yml`
- ✅ Использовать стандартный класс из SDK
- ✅ В `main.m` вызывать `[FxPrincipal startServicePrincipal]`

---

## Ручная Установка (если нужна отладка)

### Шаг 1: Очистка

```bash
# Закрыть приложения
killall Motion "Final Cut Pro"
echo "1212" | sudo -S killall -9 gPHYXFillXPC

# Удалить старые версии
echo "1212" | sudo -S rm -rf /Applications/gPHYXFill.app
rm -rf ~/Movies/Motion\ Templates.localized/Effects.localized/gPHYX\ 219

# Очистить кэши
/System/Library/Frameworks/CoreServices.framework/Frameworks/LaunchServices.framework/Support/lsregister -kill -r -domain local -domain system -domain user
echo "1212" | sudo -S killall pkd

# Отменить регистрацию
pluginkit -r -i com.gphyx.FillEffect
```

### Шаг 2: Сборка

```bash
cd /Users/abuahmad/Projects/gPHYXFill

# Генерация проекта
xcodegen generate

# Сборка Release
xcodebuild build \
    -project gPHYXFill.xcodeproj \
    -scheme gPHYXFill \
    -configuration Release \
    -derivedDataPath DerivedData
```

### Шаг 3: Упаковка

```bash
APP_PATH="DerivedData/Build/Products/Release/gPHYXFill.app"
XPC_PATH="DerivedData/Build/Products/Release/gPHYXFillXPC.pluginkit"

# Копировать XPC в PlugIns
mkdir -p "$APP_PATH/Contents/PlugIns"
cp -R "$XPC_PATH" "$APP_PATH/Contents/PlugIns/"
rm -rf "$APP_PATH/Contents/XPCServices"

# Внедрить фреймворки
FRAMEWORKS_DIR="$APP_PATH/Contents/PlugIns/gPHYXFillXPC.pluginkit/Contents/Frameworks"
mkdir -p "$FRAMEWORKS_DIR"
cp -a /Applications/Motion.app/Contents/Frameworks/FxPlug.framework "$FRAMEWORKS_DIR/"
cp -a /Applications/Motion.app/Contents/Frameworks/PluginManager.framework "$FRAMEWORKS_DIR/"

# Подпись
echo "1212" | sudo -S xattr -rc "$APP_PATH"
codesign --force --sign - --timestamp=none --deep "$FRAMEWORKS_DIR/FxPlug.framework"
codesign --force --sign - --timestamp=none --deep "$FRAMEWORKS_DIR/PluginManager.framework"
codesign --force --options runtime --sign - --timestamp=none --entitlements frontend/XPC.entitlements "$APP_PATH/Contents/PlugIns/gPHYXFillXPC.pluginkit/Contents/MacOS/gPHYXFillXPC"
codesign --force --sign - --timestamp=none "$APP_PATH/Contents/PlugIns/gPHYXFillXPC.pluginkit"
codesign --force --sign - --timestamp=none --deep --entitlements frontend/App.entitlements "$APP_PATH"
```

### Шаг 4: Установка

```bash
# Копировать в /Applications
echo "1212" | sudo -S cp -R "$APP_PATH" /Applications/
echo "1212" | sudo -S chown -R $USER:admin /Applications/gPHYXFill.app
echo "1212" | sudo -S xattr -r -d com.apple.quarantine /Applications/gPHYXFill.app

# Регистрация
/System/Library/Frameworks/CoreServices.framework/Frameworks/LaunchServices.framework/Support/lsregister -f -R -trusted /Applications/gPHYXFill.app
pluginkit -a /Applications/gPHYXFill.app/Contents/PlugIns/gPHYXFillXPC.pluginkit
pluginkit -e use -i com.gphyx.FillEffect

# Установить Motion Template
MOTION_TEMPLATES="$HOME/Movies/Motion Templates.localized/Effects.localized"
mkdir -p "$MOTION_TEMPLATES/gPHYX 220/gPHYX Fill.localized"
cp -f templates/gPHYX\ 220/gPHYX\ Fill.localized/gPHYX\ Fill\ v2.20.moef "$MOTION_TEMPLATES/gPHYX 220/gPHYX Fill.localized/"
touch "$MOTION_TEMPLATES/gPHYX 220/.localized"
touch "$MOTION_TEMPLATES/gPHYX 220/gPHYX Fill.localized/.localized"
```

### Шаг 5: Проверка

```bash
# Проверить регистрацию
pluginkit -m -v | grep com.gphyx.FillEffect

# Должно быть:
# +  com.gphyx.FillEffect(2.20)  [UUID]  /Applications/gPHYXFill.app/...
```

### Шаг 6: Запуск Motion

```bash
open -a Motion
```

---

## Отладка

### Плагин не появляется в Motion

1. **Проверить Info.plist**:
   ```bash
   plutil -p /Applications/gPHYXFill.app/Contents/PlugIns/gPHYXFillXPC.pluginkit/Contents/Info.plist | grep -A 5 "PlugInKit"
   ```
   
   Должно быть:
   - `PrincipalClass` = `FxPrincipal`
   - НЕТ ключа `com.apple.version`

2. **Проверить логи Motion**:
   ```bash
   log show --predicate 'process == "Motion"' --last 5m --info | grep -i "gphyx\|plugin\|error"
   ```

3. **Перезапустить pkd**:
   ```bash
   echo "1212" | sudo -S killall pkd
   pluginkit -a /Applications/gPHYXFill.app/Contents/PlugIns/gPHYXFillXPC.pluginkit
   ```

### Ошибки сборки

Если сборка не удалась:
```bash
# Очистить DerivedData
rm -rf ~/Library/Developer/Xcode/DerivedData/gPHYXFill-*
rm -rf DerivedData

# Пересоздать проект
xcodegen generate
```

---

## Технические Детали v2.20

- **Bundle ID**: `com.gphyx.FillEffect`
- **Версия**: `2.20`
- **Группа**: `gPHYX 220`
- **UUID**: `F88A17F8-ADE3-4261-8C13-2F0660E23360`
- **Principal**: `FxPrincipal` (стандартный из SDK)
- **Архитектура**: Swift + Objective-C++ + Metal
- **Frameworks**: FxPlug.framework, PluginManager.framework (из Motion.app)


# gPHYX Fill Plugin - Troubleshooting Guide (Путеводитель)

## Критические проверки перед установкой

### 1. Удаление ВСЕХ старых копий
Перед установкой новой версии ОБЯЗАТЕЛЬНО удалить все дубликаты:

```bash
# turbo-all
sudo rm -rf /Applications/gPHYXFill.app
sudo rm -rf /Library/Plug-Ins/FxPlug/gPHYXFill.app
rm -rf ~/Library/Application\ Scripts/com.apple.FinalCut/gPHYXFill.appex
rm -rf ~/Projects/gPHYXFill/DerivedData
rm -rf ~/Library/Developer/Xcode/DerivedData/gPHYXFill*
killall pkd
```

### 2. Песочница (Sandbox)
macOS блокирует XPC без правильных entitlements. Проверь `XPC.entitlements`:

```xml
<key>com.apple.security.app-sandbox</key>
<true/>
<key>com.apple.security.inherit</key>
<true/>
```

### 3. Identifiers (Bundle IDs)
Идентификаторы должны быть иерархическими и без версий.
- **App**: `com.gphyx.FillEffect`
- **XPC**: `com.gphyx.FillEffect.xpc`

### 4. Структура бандла
XPC должен быть в `Contents/PlugIns/`, НЕ в `Contents/XPCServices/`:

```bash
# Исправление структуры
mv "$APP/Contents/XPCServices/"* "$APP/Contents/PlugIns/"
rmdir "$APP/Contents/XPCServices"
```

### 5. Встраивание системных фреймворков
`FxPlug.framework` и `PluginManager.framework` НЕ должны встраиваться (embed: false в project.yml).

# 6. Очистка после установки (ВАЖНО)
После успешной установки в `/Applications` НЕОБХОДИМО удалить все исходники сборки, чтобы система не видела дубликатов:

```bash
# turbo-all
rm -rf "$BUILD_PATH"
rm -rf ~/Library/Developer/Xcode/DerivedData/gPHYXFill*
rm -rf ~/Projects/gPHYXFill/DerivedData
```

## Установка правильной версии

```bash
# 1. Собрать
xcodegen generate && xcodebuild -project gPHYXFill.xcodeproj -scheme gPHYXFill -configuration Debug build

# 2. Исправить структуру
BUILD="/Users/abuahmad/Library/Developer/Xcode/DerivedData/gPHYXFill-aowoygxohojjbpdysrhrzsfbfzxy/Build/Products/Debug/gPHYXFill.app"
rm -rf "$BUILD/Contents/PlugIns"
mkdir -p "$BUILD/Contents/PlugIns"
mv "$BUILD/Contents/XPCServices/"* "$BUILD/Contents/PlugIns/"
rmdir "$BUILD/Contents/XPCServices"

# 3. Установить
sudo cp -R "$BUILD" /Applications/
/System/Library/Frameworks/CoreServices.framework/Frameworks/LaunchServices.framework/Support/lsregister -f -R -trusted /Applications/gPHYXFill.app

# 4. УДАЛИТЬ "ХВОСТЫ" (Строго обязательно!)
rm -rf /Users/abuahmad/Library/Developer/Xcode/DerivedData/gPHYXFill*
rm -rf /Users/abuahmad/Projects/gPHYXFill/DerivedData

# 5. Проверить регистрацию
pluginkit -m -v | grep gphyx
```

## Текущая версия
- **Версия:** 2.19
- **Bundle ID App:** com.gphyx.FillEffect
- **Bundle ID Plugin:** com.gphyx.FillEffect.xpc
- **Путь:** /Applications/gPHYXFill.app

## Важные файлы
- `frontend/AppInfo.plist` — ID приложения
- `frontend/XPCInfo.plist` — ID плагина
- `frontend/XPC.entitlements` — права песочницы
- `project.yml` — настройки сборки и структуры
