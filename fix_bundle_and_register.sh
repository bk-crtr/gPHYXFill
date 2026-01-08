#!/bin/bash
APP_PATH=$(ls -d /Users/abuahmad/Library/Developer/Xcode/DerivedData/gPHYXFill-*/Build/Products/Debug/gPHYXFill.app | head -n 1)
echo "Fixing bundle at $APP_PATH"
PLUGINKIT_PATH="$APP_PATH/Contents/PlugIns/gPHYXFillXPC.pluginkit"

# 1. Ensure PlugIns exist and contains the pluginkit
if [ -d "$APP_PATH/Contents/XPCServices" ]; then
    echo "Moving XPCServices to PlugIns..."
    mkdir -p "$APP_PATH/Contents/PlugIns"
    cp -R "$APP_PATH/Contents/XPCServices/"* "$APP_PATH/Contents/PlugIns/"
    # rm -rf "$APP_PATH/Contents/XPCServices"
fi

# 2. Embed Frameworks
echo "Embedding FxPlug frameworks..."
mkdir -p "$PLUGINKIT_PATH/Contents/Frameworks"
cp -R "/Applications/Motion.app/Contents/Frameworks/FxPlug.framework" "$PLUGINKIT_PATH/Contents/Frameworks/"
cp -R "/Applications/Motion.app/Contents/Frameworks/PluginManager.framework" "$PLUGINKIT_PATH/Contents/Frameworks/"

# 3. Quick sign (ad-hoc)
echo "Ad-hoc signing..."
codesign --force --deep --sign - "$APP_PATH"

# 4. Register
echo "Registering..."
pluginkit -r "$PLUGINKIT_PATH" 2>/dev/null
pluginkit -a "$PLUGINKIT_PATH"

# 5. Verify
echo "Verifying..."
pluginkit -m -i com.gphyx.FillEffect
ls-register -f -R -trusted "$APP_PATH" 2>/dev/null || /System/Library/Frameworks/CoreServices.framework/Frameworks/LaunchServices.framework/Support/lsregister -f -R -trusted "$APP_PATH"
