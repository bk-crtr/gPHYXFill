#!/bin/bash
set -e

# User password for sudo operations (auto-input)
SUDO_PASSWORD="1212"

echo "🚀 gPHYX Fill v2.29 - Installation Script (Copy Strategy)"
echo "==========================================================="

# 1. NUCLEAR CLEANUP v3 - DO THIS FIRST!
echo "🧹 NUCLEAR CLEANUP v3 - Removing all old installations and caches..."

# Stop everything
killall "Motion" 2>/dev/null || true
killall "Final Cut Pro" 2>/dev/null || true
killall -9 cfprefsd 2>/dev/null || true
echo "$SUDO_PASSWORD" | sudo -S killall -9 FxPlugCacheService 2>/dev/null || true
echo "$SUDO_PASSWORD" | sudo -S killall -9 gPHYXFillXPC 2>/dev/null || true

# Clear DerivedData
rm -rf ~/Library/Developer/Xcode/DerivedData/gPHYXFill-*

# Unregister ALL possible identifiers
pluginkit -m -A 2>&1 | grep -i "FillEffect" | awk '{print $1}' | while read bundle_id; do
    echo "  Unregistering: $bundle_id"
    pluginkit -r "$bundle_id" 2>/dev/null || true
done

# Deep cleaning directories
echo "$SUDO_PASSWORD" | sudo -S rm -rf "/Library/Plug-Ins/FxPlug/gPHYXFill.app"
echo "$SUDO_PASSWORD" | sudo -S rm -rf "/Applications/gPHYXFill.app"
echo "$SUDO_PASSWORD" | sudo -S rm -rf ~/Library/Caches/com.apple.pluginkit/*
echo "$SUDO_PASSWORD" | sudo -S rm -rf ~/Library/Caches/com.apple.motionapp/*
echo "$SUDO_PASSWORD" | sudo -S rm -rf ~/Library/Caches/com.apple.FinalCut/*

# Reset LaunchServices cache
/System/Library/Frameworks/CoreServices.framework/Frameworks/LaunchServices.framework/Support/lsregister -gc -domain local -domain system -domain user
echo "✅ System cleaned up"

# 2. Build
echo "🔨 Building project v2.29..."
xcodegen generate
xcodebuild -project gPHYXFill.xcodeproj -scheme gPHYXFill -configuration Debug clean build

BUILD_SETTINGS=$(xcodebuild -project gPHYXFill.xcodeproj -scheme gPHYXFill -showBuildSettings)
BUILD_PATH=$(echo "$BUILD_SETTINGS" | grep " BUILT_PRODUCTS_DIR =" | awk -F' = ' '{print $2}')/gPHYXFill.app

# 3. Embed Frameworks into PlugInKit (COPY Strategy - safer for system)
echo "📦 Copying Frameworks (Originals)..."
PLUGINKIT_PATH="$BUILD_PATH/Contents/PlugIns/gPHYXFillXPC.pluginkit"
mkdir -p "$PLUGINKIT_PATH/Contents/Frameworks"

# Copy EVERYTHING to avoid any missing subcomponents
rm -rf "$PLUGINKIT_PATH/Contents/Frameworks/FxPlug.framework"
rm -rf "$PLUGINKIT_PATH/Contents/Frameworks/PluginManager.framework"
cp -R "/Applications/Motion.app/Contents/Frameworks/FxPlug.framework" "$PLUGINKIT_PATH/Contents/Frameworks/"
cp -R "/Applications/Motion.app/Contents/Frameworks/PluginManager.framework" "$PLUGINKIT_PATH/Contents/Frameworks/"

# 4. Verify 3-level structure
echo "🔍 Verifying 3-level FxPlug structure..."
FXPLUG_PATH="$PLUGINKIT_PATH/Contents/PlugIns/gPHYXFillXPC.fxplug"
if [ ! -d "$FXPLUG_PATH" ]; then
    echo "❌ ERROR: .fxplug bundle not found! restructure_fxplug.sh may have failed."
    exit 1
fi

# 5. Install to /Applications
echo "🚚 Installing to /Applications..."
echo "$SUDO_PASSWORD" | sudo -S cp -R "$BUILD_PATH" "/Applications/"
# CRITICAL: Use -hR to NOT follow symlinks and NOT break Motion.app
echo "$SUDO_PASSWORD" | sudo -S chown -hR $(whoami):staff "/Applications/gPHYXFill.app"

# 6. Security (Re-sign copied frameworks and binaries)
echo "🔒 Applying security fixes..."
echo "$SUDO_PASSWORD" | sudo -S xattr -rc "/Applications/gPHYXFill.app" 2>/dev/null || true

XPC_PATH="/Applications/gPHYXFill.app/Contents/PlugIns/gPHYXFillXPC.pluginkit"
FXPLUG_PATH="$XPC_PATH/Contents/PlugIns/gPHYXFillXPC.fxplug"

echo "🔒 Re-signing copied frameworks and bundles (inside-out)..."
# Sign frameworks first
codesign --force --sign "Apple Development: gadjikgs@gmail.com" "$XPC_PATH/Contents/Frameworks/FxPlug.framework/Versions/A/FxPlug"
codesign --force --sign "Apple Development: gadjikgs@gmail.com" "$XPC_PATH/Contents/Frameworks/PluginManager.framework/Versions/B/PluginManager"

# Sign .fxplug bundle (CRITICAL - was missing!)
codesign --force --sign "Apple Development: gadjikgs@gmail.com" "$FXPLUG_PATH"

# Sign XPC executable and pluginkit
codesign --force --sign "Apple Development: gadjikgs@gmail.com" "$XPC_PATH/Contents/MacOS/gPHYXFillXPC"
codesign --force --sign "Apple Development: gadjikgs@gmail.com" "$XPC_PATH"

# Sign wrapper app
codesign --force --sign "Apple Development: gadjikgs@gmail.com" "/Applications/gPHYXFill.app"

# 7. System Registration
echo "🔗 Registering with LaunchServices..."
/System/Library/Frameworks/CoreServices.framework/Frameworks/LaunchServices.framework/Support/lsregister -f -R -trusted "/Applications/gPHYXFill.app"

# 8. PlugInKit Registration
echo "🔌 Registering XPC service with PlugInKit v2.29..."
echo "$SUDO_PASSWORD" | sudo -S pluginkit -v -a "$XPC_PATH"

# 9. Launch App for XPC registration
echo "🚀 Launching app for XPC registration..."
open "/Applications/gPHYXFill.app"
sleep 3

# 10. Verify registration
echo "🔍 Verifying registration..."
PLUGIN_CHECK=$(pluginkit -m -p com.apple.FxPlug.service | grep -i "gPHYX" || echo "")
if [ -z "$PLUGIN_CHECK" ]; then
    echo "⚠️ Plugin not found by com.apple.FxPlug.service, checking FxPlug..."
    pluginkit -m -v -p FxPlug | grep -i "gPHYX\|Fill" || true
else
    echo "✅ Plugin registered: $PLUGIN_CHECK"
fi

# 11. Motion Template
echo "📄 Installing templates..."
TEMPLATE_DST="$HOME/Movies/Motion Templates.localized/Effects.localized/gPHYX Tools"
mkdir -p "$TEMPLATE_DST"
cp -R "templates/gPHYX Tools/"* "$TEMPLATE_DST/"
touch "$TEMPLATE_DST"

echo ""
echo "✅ All Done! gPHYX Fill v2.29 is installed."
echo "🎬 Please restart Motion and look for 'gPHYX Fill v2.29' in Library."
echo ""
