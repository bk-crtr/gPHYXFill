# Как сделать чтобы FxPlug плагин появился в Motion

## Быстрое решение (TL;DR)

```bash
# 1. Удалить DerivedData (Motion загружает оттуда старую версию!)
rm -rf ~/Library/Developer/Xcode/DerivedData/gPHYXFill-*

# 2. Перерегистрировать плагин из /Applications
pluginkit -r com.gphyx.FillEffect.xpc
pluginkit -a "/Applications/gPHYXFill.app/Contents/PlugIns/gPHYXFillXPC.pluginkit"

# 3. Перезапустить Motion
killall Motion; sleep 2; open -a Motion
```

---

## Полная инструкция

### 1. Проверка регистрации
```bash
pluginkit -m -v -p FxPlug | grep -i "gPHYX"
```
Должен показать путь к `/Applications/...`, НЕ к `DerivedData`.

### 2. Если путь указывает на DerivedData — УДАЛИТЬ!
```bash
rm -rf ~/Library/Developer/Xcode/DerivedData/gPHYXFill-*
```

### 3. Структура Info.plist

#### XPCInfo.plist (`.pluginkit/Contents/Info.plist`)
Обязательные ключи:
- `PlugInKit.Attributes.com.apple.protocol` = `FxPlug`
- `PlugInKit.Attributes.com.apple.version` = `4.0`
- `ProPlugPlugInList` — массив с описанием плагинов
- `ProPlugPlugInGroupList` — массив с группами
- `XPCService` с `JoinExistingSession`, `RunLoopType`, `ServiceType`

#### FxPlugInfo.plist (`.fxplug/Contents/Info.plist`)
Обязательные ключи:
- `ProPlugPlugInList` — ОБЯЗАТЕЛЬНО! Без него Motion не видит плагин.
- `ProPlugPlugInGroupList`
- `ProPlugDictionaryVersion` = `1.0`

### 4. Подпись бандлов (порядок важен!)
```bash
# Изнутри наружу:
codesign --force --sign "Apple Development: ..." "$XPC/Frameworks/FxPlug.framework/Versions/A/FxPlug"
codesign --force --sign "Apple Development: ..." "$XPC/Frameworks/PluginManager.framework/Versions/B/PluginManager"
codesign --force --sign "Apple Development: ..." "$XPC/PlugIns/gPHYXFillXPC.fxplug"  # ← Часто забывают!
codesign --force --sign "Apple Development: ..." "$XPC/MacOS/gPHYXFillXPC"
codesign --force --sign "Apple Development: ..." "$XPC"
codesign --force --sign "Apple Development: ..." "/Applications/gPHYXFill.app"
```

### 5. Финальная регистрация
```bash
# Запустить wrapper app для регистрации XPC
open /Applications/gPHYXFill.app

# Или вручную:
pluginkit -a "/Applications/gPHYXFill.app/Contents/PlugIns/gPHYXFillXPC.pluginkit"
```

### 6. Перезапуск Motion
```bash
killall Motion
open -a Motion
```

---

## Частые ошибки

| Ошибка | Решение |
|--------|---------|
| Плагин не виден | Удалить DerivedData, перерегистрировать |
| `code signature invalid` | Подписать .fxplug бандл |
| XPC крашится | Скопировать FxPlug.framework и PluginManager.framework из Motion.app |
| Motion крашится | НЕ использовать симлинки для фреймворков, копировать оригиналы |

---

## Проверка логов
```bash
log show --last 5m --predicate 'eventMessage CONTAINS "gPHYXFill"' | head -n 50
```

Если видите путь к `DerivedData` — это проблема. Удалите папку и перерегистрируйте.
