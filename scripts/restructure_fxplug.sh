#!/bin/bash
set -e

echo "🔧 Restructuring FxPlug bundle (Simple - no .fxplug needed)..."

# This script runs as postBuildScript for gPHYXFill wrapper app
if [ -z "$BUILT_PRODUCTS_DIR" ]; then
    echo "❌ Error: BUILT_PRODUCTS_DIR not set. Run this from Xcode build phase."
    exit 1
fi

WRAPPER_APP="$BUILT_PRODUCTS_DIR/gPHYXFill.app"

if [ ! -d "$WRAPPER_APP" ]; then
    echo "❌ Error: Wrapper app not found at: $WRAPPER_APP"
    exit 1
fi

# Step 1: Move .pluginkit from XPCServices to PlugIns
echo "📦 Step 1: Moving .pluginkit from XPCServices to PlugIns..."
XPC_SOURCE="$WRAPPER_APP/Contents/XPCServices/gPHYXFillXPC.pluginkit"
PLUGINS_DIR="$WRAPPER_APP/Contents/PlugIns"
PLUGINKIT_DEST="$PLUGINS_DIR/gPHYXFillXPC.pluginkit"

if [ ! -d "$XPC_SOURCE" ]; then
    echo "❌ Error: XPC service not found at: $XPC_SOURCE"
    exit 1
fi

mkdir -p "$PLUGINS_DIR"
rm -rf "$PLUGINKIT_DEST"
mv "$XPC_SOURCE" "$PLUGINKIT_DEST"
echo "✅ Moved to: $PLUGINKIT_DEST"

# Step 2: Create .fxplug bundle inside .pluginkit
echo "📁 Step 2: Creating .fxplug bundle (3rd level)..."
FXPLUG_PATH="$PLUGINKIT_DEST/Contents/PlugIns/gPHYXFillXPC.fxplug"

mkdir -p "$FXPLUG_PATH/Contents/MacOS"
mkdir -p "$FXPLUG_PATH/Contents/Resources"

# Copy FxPlug Info.plist
FXPLUG_INFO="$PROJECT_DIR/frontend/FxPlugInfo.plist"
if [ -f "$FXPLUG_INFO" ]; then
    cp "$FXPLUG_INFO" "$FXPLUG_PATH/Contents/Info.plist"
    echo "✅ Copied FxPlug Info.plist"
else
    echo "⚠️  FxPlugInfo.plist not found, using XPCInfo.plist"
    cp "$PLUGINKIT_DEST/Contents/Info.plist" "$FXPLUG_PATH/Contents/Info.plist"
fi

# Step 3: Create symlinks from .fxplug to .pluginkit resources
echo "🔗 Step 3: Creating symlinks (like gPHYXSplat)..."

# Symlink executable (gPHYXSplat uses symlink: ../../MacOS/Executable)
# Note: paradoxically, this points to .fxplug/MacOS/, but gPHYXSplat works this way.
cd "$FXPLUG_PATH/Contents/MacOS"
rm -f gPHYXFillXPC
ln -sf ../../MacOS/gPHYXFillXPC gPHYXFillXPC
echo "   ✅ Symlink: MacOS/gPHYXFillXPC → ../../MacOS/gPHYXFillXPC"

# Symlink Resources folder
# gPHYXSplat uses ../../../Resources for files inside Resources.
# Linking the folder itself with ../../../Resources points to .pluginkit/Contents/Resources.
cd "$FXPLUG_PATH/Contents"
if [ -d "$PLUGINKIT_DEST/Contents/Resources" ]; then
    rm -rf Resources
    ln -sf ../../../Resources Resources
    echo "   ✅ Symlink: Resources → ../../../Resources"
fi

# Step 4: Remove empty XPCServices folder
echo "🗑️  Step 4: Cleaning up empty XPCServices..."
if [ -d "$WRAPPER_APP/Contents/XPCServices" ]; then
    rmdir "$WRAPPER_APP/Contents/XPCServices" 2>/dev/null && echo "   ✅ Removed empty XPCServices" || echo "   ℹ️  XPCServices not empty or doesn't exist"
fi

echo ""
echo "✅ FxPlug restructuring complete!"
echo "   Final structure:"
echo "     gPHYXFill.app/"
echo "       └── Contents/PlugIns/"
echo "             └── gPHYXFillXPC.pluginkit/ (executable in Contents/MacOS/)"
echo ""
