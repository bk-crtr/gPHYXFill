---
description: Памятка по обеспечению видимости плагина gPHYX Fill v2.28
---

Если плагин «исчез» из Motion или FCP, выполните следующие шаги для гарантированного восстановления:

## Критически важно: Структура XPC-сервиса

> [!CAUTION]
> **XPC-сервис ДОЛЖЕН находиться в `XPCServices`, а НЕ в `PlugIns`!**
> Это самая частая причина, по которой плагин не регистрируется в PlugInKit.

Правильная структура:
```
gPHYXFill.app/
  Contents/
    XPCServices/              ← ПРАВИЛЬНО!
      gPHYXFillXPC.pluginkit/
    PlugIns/                  ← НЕПРАВИЛЬНО для XPC!
```

### 1. Проверка идентификаторов
Убедитесь, что `Bundle ID` в `project.yml` и `XPCInfo.plist` совпадает с текущей активной версией:
- **v2.28**: `com.gphyx.FillEffect228`
- Если вы меняете версию, **ОБЯЗАТЕЛЬНО** измените цифры в Bundle ID (например, на `229`). macOS агрессивно кеширует плагины по ID.

### 2. Глубокая очистка (Nuclear Cleanup)
// turbo
Запустите очистку вручную, если скрипт не помог:
```bash
pluginkit -r /Applications/gPHYXFill.app/Contents/PlugIns/gPHYXFillXPC.pluginkit
sudo rm -rf /Applications/gPHYXFill.app
killall -9 cfprefsd
sudo killall -9 FxPlugCacheService
```

### 3. Правильная установка
// turbo
Всегда используйте `./install_to_system.sh`. Он выполняет критические шаги:

**ВАЖНО**: Скрипт автоматически:
1. Собирает проект **БЕЗ sudo** (чтобы избежать проблем с Metal Toolchain)
2. Устанавливает **С sudo** в `/Applications/`
3. **Автоматически регистрирует** плагин через `pluginkit -a`
4. **Проверяет регистрацию** и выводит статус

```bash
# Просто запустите:
./install_to_system.sh
# Введите пароль когда попросит
```

**После установки:**
- Плагин автоматически зарегистрирован в PlugInKit
- Просто перезапустите Motion/FCP - плагин появится сразу
- Если не появился - см. раздел "Проверка регистрации" ниже

### 4. Проверка регистрации
Проверьте, видит ли система плагин:
```bash
pluginkit -m -v -p com.apple.FxPlug.XPC | grep FillEffect
```
- Если вывод пуст, проблема в подписи (codesign) или в том, что `PlugInKit` не считает бандл валидным плагином.

### 5. Motion Templates
Если плагин зарегистрирован, но его нет в списке эффектов:
- Проверьте путь: `~/Movies/Motion Templates.localized/Effects.localized/gPHYX Final 228/`
- Файл `.moef` должен иметь правильный `pluginUUID`, совпадающий с `XPCInfo.plist`.

> [!IMPORTANT]
> При любых изменениях в интерфейсе или UUID — всегда делайте "Version Bump" в Bundle ID, чтобы сбросить системный кэш.
