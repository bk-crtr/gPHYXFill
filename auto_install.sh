#!/bin/bash
# Полностью автоматическая установка gPHYX Fill v2.20
set -e

PASSWORD="1212"
VERSION="2.23"
BUNDLE_ID="com.gphyx.FillEffect"
APP_NAME="gPHYXFill.app"
XPC_NAME="gPHYXFillXPC.pluginkit"
PROJECT_NAME="gPHYXFill"
PROJECT_DIR="/Users/abuahmad/Projects/gPHYXFill"

# Цвета для вывода
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== gPHYX Fill v${VERSION} - Автоматическая Установка ===${NC}"

# 1. Закрыть Motion и Final Cut Pro
echo -e "${YELLOW}[1/10]${NC} Закрываю Motion и Final Cut Pro..."
killall "Motion" 2>/dev/null || true
killall "Final Cut Pro" 2>/dev/null || true
echo $PASSWORD | sudo -S killall -9 gPHYXFillXPC 2>/dev/null || true
sleep 2

# 2. Удалить старые версии
echo -e "${YELLOW}[2/10]${NC} Удаляю старые версии плагина..."
echo $PASSWORD | sudo -S rm -rf "/Applications/$APP_NAME"
echo $PASSWORD | sudo -S rm -rf "/Library/Plug-Ins/FxPlug/$APP_NAME"
rm -rf "$HOME/Movies/Motion Templates.localized/Effects.localized/gPHYX 219" 2>/dev/null || true
rm -rf "$HOME/Movies/Motion Templates.localized/Effects.localized/gPHYX 220" 2>/dev/null || true
rm -rf "$HOME/Movies/Motion Templates.localized/Effects.localized/gPHYX 221" 2>/dev/null || true
rm -rf "$HOME/Movies/Motion Templates.localized/Effects.localized/gPHYX 222" 2>/dev/null || true
rm -rf "$HOME/Movies/Motion Templates.localized/Effects.localized/gPHYX Tools" 2>/dev/null || true

# 3. Очистить кэши
echo -e "${YELLOW}[3/10]${NC} Очищаю системные кэши..."
/System/Library/Frameworks/CoreServices.framework/Frameworks/LaunchServices.framework/Support/lsregister -kill -r -domain local -domain system -domain user 2>/dev/null || true
echo $PASSWORD | sudo -S killall pkd 2>/dev/null || true

# 4. Отменить регистрацию старых плагинов
echo -e "${YELLOW}[4/10]${NC} Отменяю регистрацию старых версий..."
pluginkit -r -i com.gphyx.FillEffectX 2>/dev/null || true
pluginkit -r -i com.gphyx.FillEffectX18 2>/dev/null || true
pluginkit -r -i com.gphyx.FillEffectX19 2>/dev/null || true
pluginkit -r -i com.gphyx.FillEffect 2>/dev/null || true

# 5. Очистить DerivedData
echo -e "${YELLOW}[5/10]${NC} Очищаю DerivedData..."
rm -rf ~/Library/Developer/Xcode/DerivedData/${PROJECT_NAME}-* 2>/dev/null || true
rm -rf "$PROJECT_DIR/DerivedData"

# 6. Генерация проекта и сборка
echo -e "${YELLOW}[6/10]${NC} Генерирую Xcode проект..."
cd "$PROJECT_DIR"
xcodegen generate

echo -e "${YELLOW}[7/10]${NC} Собираю проект (Release)..."
LOCAL_DERIVED_DATA="$PROJECT_DIR/DerivedData"
xcodebuild build \
    -project gPHYXFill.xcodeproj \
    -scheme gPHYXFill \
    -configuration Release \
    -derivedDataPath "$LOCAL_DERIVED_DATA" \
    | grep -E "^\*\*|error:|warning:|succeeded|failed" || true

BUILD_PRODUCTS="$LOCAL_DERIVED_DATA/Build/Products/Release"
APP_PATH="$BUILD_PRODUCTS/$APP_NAME"
XPC_PATH="$BUILD_PRODUCTS/$XPC_NAME"

# Проверка успешности сборки
if [ ! -d "$APP_PATH" ]; then
    echo -e "${RED}❌ Ошибка: Сборка не удалась!${NC}"
    exit 1
fi

# 7. Упаковка приложения
echo -e "${YELLOW}[8/10]${NC} Упаковываю приложение..."
mkdir -p "$APP_PATH/Contents/PlugIns"
cp -R "$XPC_PATH" "$APP_PATH/Contents/PlugIns/"
rm -rf "$APP_PATH/Contents/XPCServices"

# Внедрение фреймворков
FRAMEWORKS_DIR="$APP_PATH/Contents/PlugIns/$XPC_NAME/Contents/Frameworks"
mkdir -p "$FRAMEWORKS_DIR"
cp -a "/Applications/Motion.app/Contents/Frameworks/FxPlug.framework" "$FRAMEWORKS_DIR/"
cp -a "/Applications/Motion.app/Contents/Frameworks/PluginManager.framework" "$FRAMEWORKS_DIR/"

# Подпись
echo $PASSWORD | sudo -S xattr -rc "$APP_PATH" 2>/dev/null || true
codesign --force --sign - --timestamp=none --deep "$FRAMEWORKS_DIR/FxPlug.framework" 2>/dev/null
codesign --force --sign - --timestamp=none --deep "$FRAMEWORKS_DIR/PluginManager.framework" 2>/dev/null
codesign --force --options runtime --sign - --timestamp=none --entitlements "frontend/XPC.entitlements" "$APP_PATH/Contents/PlugIns/$XPC_NAME/Contents/MacOS/gPHYXFillXPC" 2>/dev/null
codesign --force --sign - --timestamp=none "$APP_PATH/Contents/PlugIns/$XPC_NAME" 2>/dev/null
codesign --force --sign - --timestamp=none --deep --entitlements "frontend/App.entitlements" "$APP_PATH" 2>/dev/null

# 8. Установка в /Applications
echo -e "${YELLOW}[9/10]${NC} Устанавливаю в /Applications..."
echo $PASSWORD | sudo -S rm -rf "/Applications/$APP_NAME"
echo $PASSWORD | sudo -S cp -R "$APP_PATH" "/Applications/$APP_NAME"
echo $PASSWORD | sudo -S chown -R $USER:admin "/Applications/$APP_NAME"
echo $PASSWORD | sudo -S xattr -r -d com.apple.quarantine "/Applications/$APP_NAME" 2>/dev/null || true

# 9. Регистрация
# 9. Регистрация плагина и установка Motion Template
echo -e "${YELLOW}[10/10]${NC} Регистрирую плагин и устанавливаю Motion Template..."
/System/Library/Frameworks/CoreServices.framework/Frameworks/LaunchServices.framework/Support/lsregister -f -R -trusted "/Applications/$APP_NAME"
pluginkit -a "/Applications/$APP_NAME/Contents/PlugIns/$XPC_NAME"
pluginkit -e use -i "$BUNDLE_ID"

# Установка Motion Template
MOTION_TEMPLATES_PATH="$HOME/Movies/Motion Templates.localized/Effects.localized"
mkdir -p "$MOTION_TEMPLATES_PATH/gPHYX 223/gPHYX Fill"
cp -f "templates/gPHYX 223/gPHYX Fill/gPHYX Fill v2.23.moef" "$MOTION_TEMPLATES_PATH/gPHYX 223/gPHYX Fill/"
touch "$MOTION_TEMPLATES_PATH/gPHYX 223/.localized"

# 10. Проверка регистрации
echo ""
echo -e "${GREEN}✅ Установка завершена!${NC}"
echo ""
echo "Статус регистрации:"
pluginkit -m -v | grep "$BUNDLE_ID" || echo -e "${RED}⚠️  Плагин не найден в системе${NC}"
echo ""

# 11. Открыть Motion
echo -e "${GREEN}Открываю Motion...${NC}"
sleep 2
open -a "Motion"

echo ""
echo -e "${GREEN}=== Готово! ===${NC}"
echo "Плагин должен появиться в Motion → Библиотека → Фильтры → gPHYX 220"
