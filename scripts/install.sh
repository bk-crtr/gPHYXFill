#!/bin/bash
# scripts/install.sh - Universal Nuclear Clean & Install (v2.20)
set -e

PASSWORD="1212"
VERSION="2.20"
BUNDLE_ID="com.gphyx.FillEffect"
APP_NAME="gPHYXFill.app"
XPC_NAME="gPHYXFillXPC.pluginkit"
PROJECT_NAME="gPHYXFill"

# Controlled Paths
LOCAL_DERIVED_DATA="$(pwd)/DerivedData"
BUILD_PRODUCTS="$LOCAL_DERIVED_DATA/Build/Products/Release"
MOTION_TEMPLATES_PATH="$HOME/Movies/Motion Templates.localized/Effects.localized"

echo "=== 1. Killing Processes ==="
echo $PASSWORD | sudo -S killall -9 gPHYXFillXPC Motion "Final Cut Pro" 2>/dev/null || true

echo "=== 2. Scorched Earth Cleanup ==="
# Remove from Applications
echo $PASSWORD | sudo -S rm -rf "/Applications/$APP_NAME"
# Remove from Library FxPlug
echo $PASSWORD | sudo -S rm -rf "/Library/Plug-Ins/FxPlug/$APP_NAME"
# Remove Motion Templates that might conflict
rm -rf "$MOTION_TEMPLATES_PATH/gPHYX Tools" 2>/dev/null || true
rm -rf "$MOTION_TEMPLATES_PATH/gPHYX 220" 2>/dev/null || true
rm -rf "$MOTION_TEMPLATES_PATH/gPHYX 219" 2>/dev/null || true
mkdir -p "$MOTION_TEMPLATES_PATH/gPHYX 220"
# Remove ALL project DerivedData from system
rm -rf ~/Library/Developer/Xcode/DerivedData/${PROJECT_NAME}-* 2>/dev/null || true
# Remove local DerivedData
rm -rf "$LOCAL_DERIVED_DATA"

# Kill Launch Services and pkd cache
echo "=== 2.1 Launch Services Cleanup ==="
/System/Library/Frameworks/CoreServices.framework/Frameworks/LaunchServices.framework/Support/lsregister -kill -r -domain local -domain system -domain user || true

echo "=== 3. Unregistering all PlugInKit instances ==="
# Unregister by bundle ID to be thorough
pluginkit -r -i com.gphyx.FillEffectX || true
pluginkit -r -i com.gphyx.FillEffectX18 || true
pluginkit -r -i com.gphyx.FillEffectX19 || true
pluginkit -r -i com.gphyx.FillEffect || true
# Unregister by old UUID just in case
pluginkit -r -u 6DCCA884-B35D-47A3-8D8A-6AC095D895ED || true

echo "=== 4. Building Project (Release) ==="
xcodebuild build -project gPHYXFill.xcodeproj -scheme gPHYXFill -configuration Release -derivedDataPath "$LOCAL_DERIVED_DATA"

# The actual build product is named gPHYXFill.app
ACTUAL_APP_NAME="gPHYXFill.app"
APP_PATH="$BUILD_PRODUCTS/$ACTUAL_APP_NAME"
XPC_PATH="$BUILD_PRODUCTS/$XPC_NAME"

echo "=== 5. Packaging Application ==="
mkdir -p "$APP_PATH/Contents/PlugIns"
cp -R "$XPC_PATH" "$APP_PATH/Contents/PlugIns/"
rm -rf "$APP_PATH/Contents/XPCServices"

# Inject Frameworks
FRAMEWORKS_DIR="$APP_PATH/Contents/PlugIns/$XPC_NAME/Contents/Frameworks"
mkdir -p "$FRAMEWORKS_DIR"
cp -a "/Applications/Motion.app/Contents/Frameworks/FxPlug.framework" "$FRAMEWORKS_DIR/"
cp -a "/Applications/Motion.app/Contents/Frameworks/PluginManager.framework" "$FRAMEWORKS_DIR/"

echo "=== 6. Signing (Ad-Hoc) ==="
# De-quarantine before signing (sudo might differ)
echo $PASSWORD | sudo -S xattr -rc "$APP_PATH" || true
# 6.1 Sign Frameworks
codesign --force --sign - --timestamp=none --deep "$FRAMEWORKS_DIR/FxPlug.framework"
codesign --force --sign - --timestamp=none --deep "$FRAMEWORKS_DIR/PluginManager.framework"
# 6.2 Sign XPC Binary with Simplified Entitlements
codesign --force --options runtime --sign - --timestamp=none --entitlements "frontend/XPC.entitlements" "$APP_PATH/Contents/PlugIns/$XPC_NAME/Contents/MacOS/gPHYXFillXPC"
# 6.3 Sign the XPC Bundle (preserve binary signature)
codesign --force --sign - --timestamp=none "$APP_PATH/Contents/PlugIns/$XPC_NAME"
# 6.4 Sign the Wrapper Application with Sandbox Entitlements
codesign --force --sign - --timestamp=none --deep --entitlements "frontend/App.entitlements" "$APP_PATH"

echo "=== 7. Installing to /Applications ==="
echo $PASSWORD | sudo -S rm -rf "/Applications/$APP_NAME"
echo $PASSWORD | sudo -S cp -R "$APP_PATH" "/Applications/$APP_NAME"
echo $PASSWORD | sudo -S chown -R $USER:admin "/Applications/$APP_NAME"
echo $PASSWORD | sudo -S xattr -r -d com.apple.quarantine "/Applications/$APP_NAME" || true

echo "=== 8. System Registration ==="
# Kill pkd to force cache refresh
echo $PASSWORD | sudo -S killall pkd || true
# Force registration with Launch Services
/System/Library/Frameworks/CoreServices.framework/Frameworks/LaunchServices.framework/Support/lsregister -f -R -trusted "/Applications/$APP_NAME"
# Register PlugInKit
pluginkit -a "/Applications/$APP_NAME/Contents/PlugIns/$XPC_NAME"
pluginkit -e use -i "$BUNDLE_ID"

echo "=== 9. Installing Motion Template ==="
mkdir -p "$MOTION_TEMPLATES_PATH/gPHYX 220/gPHYX Fill.localized"
cp -f "templates/gPHYX 220/gPHYX Fill.localized/gPHYX Fill v2.20.moef" "$MOTION_TEMPLATES_PATH/gPHYX 220/gPHYX Fill.localized/"
touch "$MOTION_TEMPLATES_PATH/gPHYX 220/.localized"
touch "$MOTION_TEMPLATES_PATH/gPHYX 220/gPHYX Fill.localized/.localized"

echo "=== 10. Final Verification ==="
echo "PlugInKit status:"
pluginkit -m -v | grep $BUNDLE_ID
echo "----------------------------------------"
echo "INSTALL COMPLETE (v$VERSION)"
echo "----------------------------------------"
